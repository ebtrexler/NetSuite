// apps/test_scan_writer.cpp
//
// Unit tests for ScanWriter. No external test framework; returns 0 on
// success, nonzero on first failure. Uses a unique temp directory so
// tests can't step on each other.
//
// Run:   ./test_scan_writer

#include "scan_writer.h"
#include "ntrx_reader.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

static int g_failures = 0;

#define CHECK(cond) do {                                                     \
    if (!(cond)) {                                                           \
        std::fprintf(stderr, "  [FAIL] %s:%d: %s\n",                         \
                     __FILE__, __LINE__, #cond);                             \
        ++g_failures;                                                        \
        return;                                                              \
    }                                                                        \
} while (0)

#define CHECK_EQ(actual, expected) do {                                      \
    auto _a = (actual);                                                      \
    auto _e = (expected);                                                    \
    if (!(_a == _e)) {                                                       \
        std::fprintf(stderr,                                                 \
            "  [FAIL] %s:%d: %s != %s (got %lld vs %lld)\n",                 \
            __FILE__, __LINE__, #actual, #expected,                          \
            static_cast<long long>(_a),                                      \
            static_cast<long long>(_e));                                     \
        ++g_failures;                                                        \
        return;                                                              \
    }                                                                        \
} while (0)

static void run(const char *name, void (*test)()) {
    int before = g_failures;
    std::printf("[TEST] %s ...\n", name);
    test();
    if (g_failures == before) std::printf("       OK\n");
}

// ---------------------------------------------------------------------------
// Test helpers
// ---------------------------------------------------------------------------

/// Test-local temp dir; cleaned at the start of every run.
static std::filesystem::path test_dir() {
    static auto dir = std::filesystem::temp_directory_path() /
                      "netrex_scan_writer_tests";
    std::filesystem::create_directories(dir);
    return dir;
}

struct ReadBack {
    scan_writer_format::Header header{};
    std::vector<std::string> channel_names;
    std::vector<double> all_samples;       // full data section, row-major
    uint64_t num_scans_from_file_size = 0; // computed, crash-recovery path
};

/// Read a complete .ntrx file from disk for inspection.
static bool read_back(const std::filesystem::path& path, ReadBack& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    f.read(reinterpret_cast<char*>(&out.header), sizeof(out.header));
    if (f.gcount() != sizeof(out.header)) return false;

    out.channel_names.clear();
    for (uint32_t i = 0; i < out.header.num_channels; ++i) {
        char buf[scan_writer_format::kChannelNameBytes] = {};
        f.read(buf, sizeof(buf));
        if (f.gcount() != sizeof(buf)) return false;
        out.channel_names.emplace_back(buf);
    }

    // Seek to data offset to be safe.
    f.seekg(static_cast<std::streamoff>(out.header.data_offset),
            std::ios::beg);

    // Read everything remaining.
    const auto start = f.tellg();
    f.seekg(0, std::ios::end);
    const auto end = f.tellg();
    f.seekg(start, std::ios::beg);

    const auto data_bytes = static_cast<std::size_t>(end - start);
    out.all_samples.resize(data_bytes / sizeof(double));
    f.read(reinterpret_cast<char*>(out.all_samples.data()), data_bytes);

    if (out.header.scan_size > 0) {
        out.num_scans_from_file_size =
            out.all_samples.size() / out.header.scan_size;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

static void empty_file_has_only_header_and_names() {
    auto path = test_dir() / "empty.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 10000;
    cfg.scan_size = 3;
    cfg.channel_names = {"ai0", "Vm", "I_nA"};

    ScanWriter w(cfg);
    CHECK(w.start());
    w.stop();

    ReadBack r;
    CHECK(read_back(path, r));
    CHECK_EQ(r.header.magic, scan_writer_format::kMagic);
    CHECK_EQ(r.header.format_version, (uint32_t)1);
    CHECK_EQ(r.header.scan_size, (uint32_t)3);
    CHECK_EQ(r.header.num_channels, (uint32_t)3);
    CHECK_EQ(r.header.num_scans_written, (uint64_t)0);
    CHECK_EQ(r.header.data_offset,
             (uint64_t)scan_writer_format::data_offset_for(3));
    CHECK_EQ(r.header.sample_rate_hz, 10000.0);
    CHECK(r.header.start_time_unix_ns > 0);

    CHECK_EQ(r.channel_names.size(), (std::size_t)3);
    CHECK(r.channel_names[0] == "ai0");
    CHECK(r.channel_names[1] == "Vm");
    CHECK(r.channel_names[2] == "I_nA");

    CHECK_EQ(r.all_samples.size(), (std::size_t)0);
    CHECK_EQ(r.num_scans_from_file_size, (uint64_t)0);
}

static void single_chunk_roundtrip() {
    auto path = test_dir() / "single_chunk.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 5000;
    cfg.scan_size = 2;
    cfg.channel_names = {"ch0", "ch1"};

    ScanWriter w(cfg);
    CHECK(w.start());

    // 4 scans × 2 channels = 8 doubles
    ScanWriter::Chunk chunk;
    chunk.num_scans = 4;
    chunk.samples = {1.0, -1.0,
                     2.0, -2.0,
                     3.0, -3.0,
                     4.0, -4.0};
    CHECK(w.submit(std::move(chunk)));

    // Give the writer thread a moment, then stop cleanly.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    w.stop();

    CHECK(w.ok());
    CHECK_EQ(w.scans_written(), (uint64_t)4);

    ReadBack r;
    CHECK(read_back(path, r));
    CHECK_EQ(r.header.num_scans_written, (uint64_t)4);   // patched on close
    CHECK_EQ(r.num_scans_from_file_size, (uint64_t)4);
    CHECK_EQ(r.all_samples.size(), (std::size_t)8);

    const std::vector<double> expected = {1.0,-1.0, 2.0,-2.0,
                                           3.0,-3.0, 4.0,-4.0};
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK_EQ(r.all_samples[i], expected[i]);
    }
}

static void many_chunks_mt() {
    auto path = test_dir() / "many_chunks.ntrx";
    std::filesystem::remove(path);

    constexpr uint32_t kScanSize = 4;
    constexpr int kNumChunks = 500;
    constexpr uint32_t kScansPerChunk = 128;

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 50000;
    cfg.scan_size = kScanSize;
    cfg.channel_names = {"a", "b", "c", "d"};
    cfg.queue_capacity = 32;

    ScanWriter w(cfg);
    CHECK(w.start());

    std::thread producer([&]() {
        uint64_t scan_idx = 0;
        for (int c = 0; c < kNumChunks; ++c) {
            ScanWriter::Chunk chunk;
            chunk.num_scans = kScansPerChunk;
            chunk.samples.resize(kScansPerChunk * kScanSize);
            // Fill with a pattern we can later verify: sample value =
            // global scan index in the run.
            for (uint32_t s = 0; s < kScansPerChunk; ++s) {
                for (uint32_t ch = 0; ch < kScanSize; ++ch) {
                    chunk.samples[s * kScanSize + ch] =
                        static_cast<double>(scan_idx + s) * 10 + ch;
                }
            }
            scan_idx += kScansPerChunk;

            while (!w.submit(std::move(chunk))) {
                // Queue was full; writer thread will catch up soon.
                std::this_thread::sleep_for(std::chrono::microseconds(200));
                // submit() preserved the moved-from value's state as
                // empty; we need to rebuild the chunk. Re-seed it:
                chunk.num_scans = kScansPerChunk;
                chunk.samples.resize(kScansPerChunk * kScanSize);
                for (uint32_t s = 0; s < kScansPerChunk; ++s) {
                    for (uint32_t ch = 0; ch < kScanSize; ++ch) {
                        chunk.samples[s * kScanSize + ch] =
                            static_cast<double>(scan_idx - kScansPerChunk + s)
                            * 10 + ch;
                    }
                }
            }
        }
    });

    producer.join();
    w.stop();

    CHECK(w.ok());
    const uint64_t expected_scans =
        static_cast<uint64_t>(kNumChunks) * kScansPerChunk;
    CHECK_EQ(w.scans_written(), expected_scans);

    ReadBack r;
    CHECK(read_back(path, r));
    CHECK_EQ(r.header.num_scans_written, expected_scans);
    CHECK_EQ(r.num_scans_from_file_size, expected_scans);
    CHECK_EQ(r.all_samples.size(),
             static_cast<std::size_t>(expected_scans) * kScanSize);

    // Spot-check the first, middle, and last scans.
    auto check_scan = [&](uint64_t scan_idx) {
        for (uint32_t ch = 0; ch < kScanSize; ++ch) {
            double expected = static_cast<double>(scan_idx) * 10 + ch;
            double got = r.all_samples[scan_idx * kScanSize + ch];
            if (got != expected) {
                std::fprintf(stderr,
                    "  [FAIL] scan %llu ch %u: expected %f got %f\n",
                    (unsigned long long)scan_idx, ch, expected, got);
                ++g_failures;
            }
        }
    };
    check_scan(0);
    check_scan(expected_scans / 2);
    check_scan(expected_scans - 1);
}

static void queue_full_produces_backpressure() {
    auto path = test_dir() / "backpressure.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 1000;
    cfg.scan_size = 1;
    cfg.channel_names = {"ch0"};
    // Intentionally tiny queue. We'll dump scans in as fast as possible.
    cfg.queue_capacity = 2;
    // Big flush interval so the writer sleeps between drains and we get
    // a real chance at filling.
    cfg.flush_interval = std::chrono::milliseconds(500);

    ScanWriter w(cfg);
    CHECK(w.start());

    uint64_t submitted = 0;
    uint64_t rejected = 0;
    // Try very hard to fill the queue. With sleeps in the writer loop
    // (1ms), a tight loop of ~100 submits should catch at least a few.
    for (int i = 0; i < 10000; ++i) {
        ScanWriter::Chunk c;
        c.num_scans = 1;
        c.samples = {static_cast<double>(i)};
        if (w.submit(std::move(c))) ++submitted; else ++rejected;
    }

    w.stop();

    CHECK(w.ok());           // no fatal error; rejections are not errors
    CHECK(submitted > 0);
    // We expect at least some rejections given kSize = 2.
    CHECK(rejected > 0);
    CHECK_EQ(w.submits_rejected(), rejected);
    CHECK_EQ(w.scans_written(), submitted);  // whatever we got in, got written

    // Note: this test doesn't assert data contents — the rejected items
    // were never written, which is the desired guaranteed-delivery shape.
}

static void submit_after_stop_is_rejected() {
    auto path = test_dir() / "reject_after_stop.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 1000;
    cfg.scan_size = 1;
    cfg.channel_names = {"x"};

    ScanWriter w(cfg);
    CHECK(w.start());
    w.stop();

    ScanWriter::Chunk c;
    c.num_scans = 1;
    c.samples = {42.0};
    CHECK(!w.submit(std::move(c)));
}

static void invalid_config_rejected() {
    auto path = test_dir() / "bad_cfg.ntrx";
    std::filesystem::remove(path);

    // Mismatched channel-names count
    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 1000;
    cfg.scan_size = 3;
    cfg.channel_names = {"only_one"};
    ScanWriter w(cfg);
    CHECK(!w.start());
    CHECK(!w.last_error().empty());
}

// Simulate the crash-recovery path: we can't easily kill the process
// mid-run inside a unit test, but we can reproduce the on-disk state
// that a crash would produce — num_scans_written = 0 — by patching the
// header back to 0 after a clean close and verifying a reader can still
// compute the true scan count from the file size.
static void crash_recovery_by_file_size() {
    auto path = test_dir() / "crash_like.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 2000;
    cfg.scan_size = 2;
    cfg.channel_names = {"a", "b"};

    ScanWriter w(cfg);
    CHECK(w.start());

    for (int c = 0; c < 5; ++c) {
        ScanWriter::Chunk chunk;
        chunk.num_scans = 10;
        chunk.samples.resize(20, static_cast<double>(c));
        CHECK(w.submit(std::move(chunk)));
    }
    w.stop();

    // Overwrite the num_scans_written field with 0 to simulate an
    // unclean termination where stop() never got to patch the header.
    {
        std::FILE* fp = std::fopen(cfg.path.c_str(), "r+b");
        CHECK(fp != nullptr);
        const long off = offsetof(scan_writer_format::Header,
                                  num_scans_written);
        CHECK_EQ(std::fseek(fp, off, SEEK_SET), 0);
        uint64_t zero = 0;
        CHECK_EQ(std::fwrite(&zero, sizeof(zero), 1, fp), (std::size_t)1);
        std::fclose(fp);
    }

    ReadBack r;
    CHECK(read_back(path, r));
    CHECK_EQ(r.header.num_scans_written, (uint64_t)0);       // "crashed"
    CHECK_EQ(r.num_scans_from_file_size, (uint64_t)50);      // recoverable
}

// Reader roundtrip: write a file with ScanWriter, read it with
// NtrxReader, and verify header, channel names, and every scan value.
static void reader_roundtrip_with_writer() {
    auto path = test_dir() / "reader_roundtrip.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 2000;
    cfg.scan_size = 3;
    cfg.channel_names = {"ai0", "Cell,with,comma", "quoted\"name"};

    ScanWriter w(cfg);
    CHECK(w.start());

    // 3 chunks × 5 scans = 15 scans
    for (int c = 0; c < 3; ++c) {
        ScanWriter::Chunk chunk;
        chunk.num_scans = 5;
        chunk.samples.resize(15);
        for (int s = 0; s < 5; ++s) {
            chunk.samples[s * 3 + 0] = c * 10.0 + s;     // ai0
            chunk.samples[s * 3 + 1] = -c * 1.5;          // col 1
            chunk.samples[s * 3 + 2] = 3.14 * (c + 1);    // col 2
        }
        CHECK(w.submit(std::move(chunk)));
    }
    w.stop();

    NtrxReader r;
    CHECK(r.open(path.string()));
    CHECK_EQ(r.info().scan_size, (uint32_t)3);
    CHECK_EQ(r.info().num_channels, (uint32_t)3);
    CHECK_EQ(r.info().num_scans, (uint64_t)15);
    CHECK_EQ(r.info().sample_rate_hz, 2000.0);
    CHECK(r.info().channel_names[0] == "ai0");
    CHECK(r.info().channel_names[1] == "Cell,with,comma");
    CHECK(r.info().channel_names[2] == "quoted\"name");

    std::vector<double> row;
    for (int c = 0; c < 3; ++c) {
        for (int s = 0; s < 5; ++s) {
            CHECK(r.read_next(row));
            CHECK_EQ(row[0], (double)(c * 10 + s));
            CHECK_EQ(row[1], (double)(-c * 1.5));
            CHECK_EQ(row[2], 3.14 * (c + 1));
        }
    }
    // EOF
    CHECK(!r.read_next(row));
}

// Reader survival: open a file whose header has num_scans_written = 0
// (the crash-like path) and verify the reader still reports the correct
// scan count from file size.
static void reader_recovers_from_zero_num_scans() {
    auto path = test_dir() / "reader_recovery.ntrx";
    std::filesystem::remove(path);

    ScanWriter::Config cfg;
    cfg.path = path.string();
    cfg.sample_rate_hz = 1000;
    cfg.scan_size = 2;
    cfg.channel_names = {"a", "b"};
    ScanWriter w(cfg);
    CHECK(w.start());
    for (int c = 0; c < 4; ++c) {
        ScanWriter::Chunk chunk;
        chunk.num_scans = 3;
        chunk.samples = {double(c*10), double(c*10+1),
                         double(c*10+2), double(c*10+3),
                         double(c*10+4), double(c*10+5)};
        CHECK(w.submit(std::move(chunk)));
    }
    w.stop();

    // Patch num_scans to 0 (simulate crash).
    {
        std::FILE* fp = std::fopen(cfg.path.c_str(), "r+b");
        CHECK(fp != nullptr);
        std::fseek(fp, offsetof(scan_writer_format::Header,
                                num_scans_written), SEEK_SET);
        uint64_t zero = 0;
        std::fwrite(&zero, sizeof(zero), 1, fp);
        std::fclose(fp);
    }

    NtrxReader r;
    CHECK(r.open(path.string()));
    CHECK_EQ(r.info().num_scans, (uint64_t)12);  // 4 × 3 recovered from size

    // Still readable end to end.
    std::vector<double> row;
    int count = 0;
    while (r.read_next(row)) ++count;
    CHECK_EQ(count, 12);
}

// ---------------------------------------------------------------------------
int main() {
    std::printf("NETrex ScanWriter tests\n");
    std::printf("=======================\n\n");

    // Fresh temp dir
    auto dir = test_dir();
    std::error_code ec;
    for (auto& entry : std::filesystem::directory_iterator(dir, ec)) {
        std::filesystem::remove(entry.path(), ec);
    }

    run("empty file has only header + names",
        empty_file_has_only_header_and_names);
    run("single chunk roundtrip",           single_chunk_roundtrip);
    run("many chunks MT",                   many_chunks_mt);
    run("queue full produces backpressure", queue_full_produces_backpressure);
    run("submit after stop is rejected",    submit_after_stop_is_rejected);
    run("invalid config rejected",          invalid_config_rejected);
    run("crash recovery by file size",      crash_recovery_by_file_size);
    run("reader roundtrip with writer",      reader_roundtrip_with_writer);
    run("reader recovers from zero num_scans", reader_recovers_from_zero_num_scans);

    std::printf("\n");
    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    }
    std::printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
