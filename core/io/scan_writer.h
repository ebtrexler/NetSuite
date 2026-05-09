// core/io/scan_writer.h
//
// Background disk-writer thread for the real-time DAQ pipeline. Owns a
// bounded SPSC queue of scan chunks and a std::thread that drains them
// to a binary .ntrx file.
//
// The producer (the DAQ thread, in the real pipeline) hands off chunks
// via `submit()`. If the writer is falling behind and the internal queue
// fills, `submit()` returns false; the caller is expected to treat that
// as a hard error ("the disk cannot keep up") rather than silently drop
// data.
//
// The writer does not know or care where the chunks come from, which is
// why this slice is independent of the DAQ layer — it can be exercised
// by unit tests with a synthetic producer.
//
// See Developer_Guide.md §4.5 for the pipeline context.
//
// ================================================================
// BINARY FILE FORMAT (.ntrx, format version 1)
// ================================================================
//
// All values little-endian. Total fixed overhead before data:
//     256 (header) + 64 * num_channels (channel-name table)
//
// Header (bytes 0..255):
//   offset  size  field                meaning
//   ------  ----  -------------------  --------------------------------
//    0      4    magic                 'NTRX' (0x4E 0x54 0x52 0x58)
//    4      4    format_version        uint32 = 1
//    8      8    sample_rate_hz        float64
//   16      4    scan_size             uint32, doubles per scan row
//   20      4    num_channels          uint32, equal to scan_size today
//                                      (split out for future use: e.g.
//                                      interleaving AI + derived state)
//   24      8    start_time_unix_ns    int64
//   32      8    num_scans_written     uint64, patched on clean close;
//                                      stays 0 if the process crashes
//                                      (reader can recover from file size)
//   40      8    data_offset           uint64, byte offset of first scan
//   48      4    flags                 uint32, bitmask (none defined yet)
//   52    204    reserved              zero
//
// Channel-name table (bytes 256 .. 256 + 64*num_channels):
//   One 64-byte NUL-padded UTF-8 string per channel, in scan-order.
//
// Data section (starting at data_offset):
//   Row-major, scan-major. Each row is exactly `scan_size` float64
//   values. Total rows = (file_size - data_offset) / (scan_size * 8).
//
// Crash recovery:
//   If the process is killed before ScanWriter::stop() has a chance to
//   patch num_scans_written, that field is 0. Readers compute the true
//   scan count from file size; no data is lost beyond whatever was still
//   in the OS page cache at the moment of death. On a clean close,
//   num_scans_written is authoritative.

#ifndef SCAN_WRITER_H
#define SCAN_WRITER_H

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "spsc_queue.h"

// ---------------------------------------------------------------------------
// On-disk constants. Exposed so readers (and tests) can use them directly.
// ---------------------------------------------------------------------------
namespace scan_writer_format {

constexpr uint32_t kMagic         = 0x5852544E;  // 'NTRX' little-endian
constexpr uint32_t kVersion       = 1;
constexpr std::size_t kHeaderBytes       = 256;
constexpr std::size_t kChannelNameBytes  = 64;   // per channel, NUL-padded

// Compute the byte offset to the first scan for a given num_channels.
inline std::size_t data_offset_for(uint32_t num_channels) {
    return kHeaderBytes + kChannelNameBytes * num_channels;
}

// POD header laid out to match the on-disk layout. Used for read/write
// convenience; callers should still treat fields as little-endian.
#pragma pack(push, 1)
struct Header {
    uint32_t magic;
    uint32_t format_version;
    double   sample_rate_hz;
    uint32_t scan_size;
    uint32_t num_channels;
    int64_t  start_time_unix_ns;
    uint64_t num_scans_written;
    uint64_t data_offset;
    uint32_t flags;
    uint8_t  reserved[204];
};
#pragma pack(pop)

static_assert(sizeof(Header) == kHeaderBytes,
              "Header must be exactly 256 bytes");

}  // namespace scan_writer_format

// ---------------------------------------------------------------------------
// ScanWriter
// ---------------------------------------------------------------------------

/// A background thread that streams scan chunks to a binary .ntrx file.
///
/// Typical lifecycle:
/// @code
///   ScanWriter::Config cfg;
///   cfg.path = "run_42.ntrx";
///   cfg.sample_rate_hz = 10000;
///   cfg.scan_size = 3;
///   cfg.channel_names = {"Dev1/ai0", "Cell_Vm", "Electrode_I"};
///
///   ScanWriter w(cfg);
///   if (!w.start()) { /* disk error */ }
///
///   // --- producer ---
///   ScanWriter::Chunk chunk;
///   chunk.samples = std::move(scanBuffer);   // size must be N*scan_size
///   chunk.num_scans = N;
///   if (!w.submit(std::move(chunk))) {
///       // disk queue full → abort run
///   }
///
///   // --- shutdown ---
///   w.stop();                                // flushes and closes
/// @endcode
class ScanWriter {
public:
    struct Config {
        std::string path;
        double sample_rate_hz = 0.0;
        uint32_t scan_size = 0;
        std::vector<std::string> channel_names;
        // Max number of chunks the submit-side queue can hold. A full
        // queue forces submit() to return false, signaling "disk can't
        // keep up". Roughly queue_capacity * avg_chunk_duration should
        // be ≥ worst-case disk stall you want to absorb (e.g. 32 chunks
        // × 100 ms = 3.2 s of tolerance).
        std::size_t queue_capacity = 64;
        // Size in bytes of the stdio I/O buffer (setvbuf). Larger =
        // fewer syscalls, at the cost of larger potential data loss on
        // a crash before fflush.
        std::size_t io_buffer_bytes = 1 << 20;  // 1 MiB
        // Flush (fflush) interval for the writer thread. On Linux, a
        // future enhancement can add fdatasync at this cadence.
        std::chrono::milliseconds flush_interval{100};
    };

    /// Unit of producer → writer hand-off. `samples.size()` must equal
    /// `num_scans * Config::scan_size`. Default constructible for the
    /// SpscQueue slot-reset idiom; moved on submit.
    struct Chunk {
        std::vector<double> samples;
        uint32_t num_scans = 0;

        Chunk() = default;
        Chunk(std::vector<double> s, uint32_t n)
            : samples(std::move(s)), num_scans(n) {}
    };

    explicit ScanWriter(Config cfg);
    ~ScanWriter();

    ScanWriter(const ScanWriter&) = delete;
    ScanWriter& operator=(const ScanWriter&) = delete;

    /// Opens the file, writes the header, and launches the writer thread.
    /// Returns false (and sets last_error()) on file-open failure.
    bool start();

    /// Signals the writer to drain the queue, flushes, patches the
    /// header's num_scans_written field, closes the file, and joins
    /// the writer thread. Safe to call multiple times.
    void stop();

    /// Producer-side enqueue. Returns false if the queue is full OR if
    /// the writer thread has hit a fatal I/O error. The caller should
    /// treat false as "abort the run"; the contents of `chunk` are
    /// preserved if the push failed.
    bool submit(Chunk&& chunk);

    // --- Diagnostics (safe to call from any thread) -------------------

    /// Total scans successfully handed to fwrite() so far.
    uint64_t scans_written() const { return m_scans_written.load(); }

    /// Number of submit() calls that failed because the queue was full.
    /// In a healthy run this stays 0.
    uint64_t submits_rejected() const { return m_submits_rejected.load(); }

    /// False if the writer thread has hit a fatal error (disk full,
    /// write returned short, etc.). Once false, it stays false and
    /// submit() will also start rejecting chunks.
    bool ok() const { return m_ok.load(); }

    /// Human-readable description of the last error. Empty if ok().
    std::string last_error() const;

private:
    void writer_loop();
    bool write_header();
    bool write_channel_names();
    bool write_chunk(const Chunk& c);
    void set_error(std::string msg);

    Config m_cfg;
    std::FILE* m_fp = nullptr;
    std::unique_ptr<char[]> m_iobuf;

    std::unique_ptr<SpscQueue<Chunk>> m_queue;
    std::thread m_thread;

    std::atomic<bool> m_stopping{false};
    std::atomic<bool> m_ok{true};
    std::atomic<bool> m_started{false};
    std::atomic<uint64_t> m_scans_written{0};
    std::atomic<uint64_t> m_submits_rejected{0};

    mutable std::mutex m_err_mu;
    std::string m_last_error;
};

#endif  // SCAN_WRITER_H
