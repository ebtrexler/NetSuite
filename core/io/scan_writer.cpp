// core/io/scan_writer.cpp
//
// Threaded disk writer. See scan_writer.h for format docs and the
// rationale behind the thread model.

#include "scan_writer.h"

#include <chrono>
#include <cstring>
#include <cerrno>
#include <utility>

namespace {

int64_t unix_nanos_now() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(
        system_clock::now().time_since_epoch()).count();
}

std::string errno_str() {
    // Use the thread-safe strerror_r where available. On macOS the XSI
    // variant returns int; on glibc the GNU variant returns char*. Pick
    // the portable-but-dumb route: format the number.
    return std::string("errno=") + std::to_string(errno);
}

}  // namespace

// ---------------------------------------------------------------------------
ScanWriter::ScanWriter(Config cfg) : m_cfg(std::move(cfg)) {}

ScanWriter::~ScanWriter() {
    stop();
}

bool ScanWriter::start() {
    if (m_started.load()) return false;  // refuse double-start

    if (m_cfg.scan_size == 0) {
        set_error("scan_size must be > 0");
        return false;
    }
    if (m_cfg.channel_names.size() != m_cfg.scan_size) {
        set_error("channel_names.size() (" +
                  std::to_string(m_cfg.channel_names.size()) +
                  ") must equal scan_size (" +
                  std::to_string(m_cfg.scan_size) + ")");
        return false;
    }
    if (m_cfg.sample_rate_hz <= 0) {
        set_error("sample_rate_hz must be > 0");
        return false;
    }

    // Open the file. "wb" truncates; that's what the user asked for by
    // choosing this path.
    m_fp = std::fopen(m_cfg.path.c_str(), "wb");
    if (!m_fp) {
        set_error("fopen(" + m_cfg.path + "): " + errno_str());
        return false;
    }

    // Big buffered I/O — fewer syscalls in the writer's hot loop.
    m_iobuf.reset(new char[m_cfg.io_buffer_bytes]);
    std::setvbuf(m_fp, m_iobuf.get(), _IOFBF, m_cfg.io_buffer_bytes);

    if (!write_header()) return false;
    if (!write_channel_names()) return false;

    m_queue.reset(new SpscQueue<Chunk>(m_cfg.queue_capacity));
    m_stopping.store(false);
    m_ok.store(true);
    m_scans_written.store(0);
    m_submits_rejected.store(0);

    m_thread = std::thread(&ScanWriter::writer_loop, this);
    m_started.store(true);
    return true;
}

void ScanWriter::stop() {
    if (!m_started.load()) return;

    m_stopping.store(true);
    if (m_thread.joinable()) m_thread.join();

    if (m_fp) {
        // Patch the scan count into the header so readers don't have to
        // derive it from file size on a clean close.
        if (m_ok.load()) {
            std::fflush(m_fp);
            const uint64_t n = m_scans_written.load();
            const long off = offsetof(scan_writer_format::Header,
                                      num_scans_written);
            if (std::fseek(m_fp, off, SEEK_SET) == 0) {
                std::fwrite(&n, sizeof(n), 1, m_fp);
            }
            std::fflush(m_fp);
        }
        std::fclose(m_fp);
        m_fp = nullptr;
    }
    m_iobuf.reset();
    m_queue.reset();
    m_started.store(false);
}

bool ScanWriter::submit(Chunk&& chunk) {
    if (!m_started.load() || !m_ok.load()) {
        return false;
    }
    // Size-consistency check. A mismatched chunk almost certainly
    // indicates a producer bug; fail loud rather than write garbage.
    const std::size_t expected =
        static_cast<std::size_t>(chunk.num_scans) * m_cfg.scan_size;
    if (chunk.samples.size() != expected) {
        set_error("chunk size mismatch: got " +
                  std::to_string(chunk.samples.size()) + ", expected " +
                  std::to_string(expected));
        m_ok.store(false);
        return false;
    }

    if (!m_queue->try_push(std::move(chunk))) {
        m_submits_rejected.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    return true;
}

std::string ScanWriter::last_error() const {
    std::lock_guard<std::mutex> g(m_err_mu);
    return m_last_error;
}

// ---------------------------------------------------------------------------
// Writer thread body.
// ---------------------------------------------------------------------------
void ScanWriter::writer_loop() {
    auto last_flush = std::chrono::steady_clock::now();

    while (true) {
        Chunk c;
        if (m_queue->try_pop(c)) {
            if (!write_chunk(c)) return;   // fatal error — stop
            continue;                      // drain as fast as possible
        }

        // Queue is empty. If we're shutting down, we're done.
        if (m_stopping.load(std::memory_order_acquire)) {
            std::fflush(m_fp);
            return;
        }

        // Periodic flush even when idle, so a late crash doesn't lose
        // everything that was still in the stdio buffer.
        auto now = std::chrono::steady_clock::now();
        if (now - last_flush >= m_cfg.flush_interval) {
            std::fflush(m_fp);
            last_flush = now;
        }

        // Nothing to do right now — yield briefly. We intentionally do
        // NOT use a condition variable; the DAQ-side producer is
        // lock-free and we never want it to have to signal anything.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool ScanWriter::write_header() {
    using namespace scan_writer_format;
    Header h{};
    h.magic = kMagic;
    h.format_version = kVersion;
    h.sample_rate_hz = m_cfg.sample_rate_hz;
    h.scan_size = m_cfg.scan_size;
    h.num_channels = m_cfg.scan_size;    // same thing for now
    h.start_time_unix_ns = unix_nanos_now();
    h.num_scans_written = 0;             // patched on clean close
    h.data_offset = data_offset_for(m_cfg.scan_size);
    h.flags = 0;

    if (std::fwrite(&h, sizeof(h), 1, m_fp) != 1) {
        set_error("fwrite(header): " + errno_str());
        m_ok.store(false);
        if (m_fp) { std::fclose(m_fp); m_fp = nullptr; }
        return false;
    }
    return true;
}

bool ScanWriter::write_channel_names() {
    char buf[scan_writer_format::kChannelNameBytes];
    for (const auto& name : m_cfg.channel_names) {
        std::memset(buf, 0, sizeof(buf));
        const std::size_t n = std::min(name.size(), sizeof(buf) - 1);
        std::memcpy(buf, name.data(), n);
        if (std::fwrite(buf, sizeof(buf), 1, m_fp) != 1) {
            set_error("fwrite(channel_name): " + errno_str());
            m_ok.store(false);
            if (m_fp) { std::fclose(m_fp); m_fp = nullptr; }
            return false;
        }
    }
    return true;
}

bool ScanWriter::write_chunk(const Chunk& c) {
    if (c.num_scans == 0) return true;  // no-op
    const std::size_t doubles = c.samples.size();
    const std::size_t n_written = std::fwrite(
        c.samples.data(), sizeof(double), doubles, m_fp);
    if (n_written != doubles) {
        set_error("fwrite(chunk): short write, wrote " +
                  std::to_string(n_written) + " of " +
                  std::to_string(doubles) + " doubles, " + errno_str());
        m_ok.store(false);
        return false;
    }
    m_scans_written.fetch_add(c.num_scans, std::memory_order_relaxed);
    return true;
}

void ScanWriter::set_error(std::string msg) {
    std::lock_guard<std::mutex> g(m_err_mu);
    if (m_last_error.empty()) m_last_error = std::move(msg);
}
