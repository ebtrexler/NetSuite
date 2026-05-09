// apps/test_daq_thread.cpp
//
// End-to-end test of DaqThread against DAQMock. Builds a minimal network
// (one TBiologicalCell with an HH current), runs the DAQ thread for
// ~500 ms at 5 kHz, and asserts that:
//   • the thread completed without error
//   • the scan count is consistent with the configured sample rate
//   • the disk file contains the expected number of records
//   • the on_chunk UI callback fired for every chunk
//
// Run:   ./test_daq_thread

#include "daq_thread.h"
#include "daq_mock.h"
#include "scan_writer.h"

#include "RT_Network.h"
#include "RT_BiologicalCell.h"
#include "RT_HHCurrent.h"
#include "factory.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

static int g_failures = 0;

#define CHECK(cond) do {                                                     \
    if (!(cond)) {                                                           \
        std::fprintf(stderr, "  [FAIL] %s:%d: %s\n",                         \
                     __FILE__, __LINE__, #cond);                             \
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

/// Register the handful of factory types we need for the network. Guard
/// with a local once-flag — duplicate registrations throw.
static void register_factories_once() {
    static bool done = false;
    if (done) return;
    done = true;
    try {
        GetCellFactory().registerBuilder(
            TBiologicalCell_KEY,
            TypeID<TBiologicalCell>(),
            TypeID<const std::wstring>());
    } catch (...) {}
    // THHCurrent registers itself in a static initializer in RT_HHCurrent.cpp
}

/// Build a network with one TBiologicalCell wired to DAQMock's mock AI/AO.
static std::unique_ptr<TNetwork> make_test_network() {
    register_factories_once();
    auto net = std::make_unique<TNetwork>(L"DaqThreadTest");
    net->SetMaxRK4Timestep(0.01);

    TCell *cell = net->AddCellToNet(TBiologicalCell_KEY, L"Bio1", 0, 0);
    cell->SetAIChannel(L"MockDev1/ai0");
    cell->SetAOChannel(L"MockDev1/ao0");
    cell->SetAIGain(1000.0);   // 1 V = 1000 mV   (mock returns V = Vm/1000)
    cell->SetAOGain(1.0);      // 1 V / nA

    auto *hh = new THHCurrent(cell, L"HH1");
    hh->Gmax(5.0);
    hh->E(-60);
    hh->p(3);
    hh->q(1);
    hh->get_m().V0(-40); hh->get_m().k(5);  hh->get_m().t_lo(1); hh->get_m().t_hi(0.5); hh->get_m().w(1); hh->get_m().infMin(0);
    hh->get_h().V0(-60); hh->get_h().k(-5); hh->get_h().t_lo(1); hh->get_h().t_hi(5);   hh->get_h().w(1); hh->get_h().infMin(0);
    cell->AddCurrent(hh);

    net->DescribeNetwork();
    net->Initialize(true);
    return net;
}

static std::filesystem::path test_dir() {
    auto dir = std::filesystem::temp_directory_path() /
               "netrex_daq_thread_tests";
    std::filesystem::create_directories(dir);
    return dir;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

static void daq_thread_runs_against_mock() {
    auto net = make_test_network();

    DAQMock daq;
    double sample_rate = daq.configureAI("MockDev1/ai0", 5000.0);
    daq.configureAO("MockDev1/ao0");
    CHECK(sample_rate > 0);

    auto path = test_dir() / "daq_thread.ntrx";
    std::filesystem::remove(path);

    // scan_size = numAI(1) + numCells(1) + numVDep(1) = 3
    ScanWriter::Config wcfg;
    wcfg.path = path.string();
    wcfg.sample_rate_hz = sample_rate;
    wcfg.scan_size = 3;
    wcfg.channel_names = {"MockDev1/ai0", "Bio1_Vm", "Bio1_I"};
    // Production-realistic queue: absorbs ≥1 s of disk stall at the
    // chunk rates real NI-DAQmx hardware produces (~25-250 chunks/s).
    wcfg.queue_capacity = 1024;
    ScanWriter writer(wcfg);
    CHECK(writer.start());

    // Capture the chunk count from the callback.
    std::atomic<int> chunk_callbacks{0};
    std::atomic<double> last_vm{0};
    std::atomic<double> last_i {0};

    DaqThread::Config cfg;
    cfg.daq = &daq;
    cfg.net = net.get();
    cfg.writer = &writer;
    cfg.sample_rate_hz = sample_rate;
    cfg.num_ai_channels = 1;
    cfg.num_cells = 1;
    cfg.num_vdep_cells = 1;
    cfg.sec_before = 0.0;
    cfg.sec_during = 10.0;       // plenty, we'll stop early
    cfg.sec_after  = 0.0;
    cfg.max_scans_per_read = 1024;

    struct Ctx {
        std::atomic<int> *chunks;
        std::atomic<double> *vm;
        std::atomic<double> *ii;
    } ctx{&chunk_callbacks, &last_vm, &last_i};
    cfg.on_chunk_ctx = &ctx;
    cfg.on_chunk = [](void* c, double /*t*/, int32_t /*n*/,
                      const double* /*ai*/,
                      const double* vm_out,
                      const double* i_na) {
        auto* cx = static_cast<Ctx*>(c);
        cx->chunks->fetch_add(1);
        cx->vm->store(vm_out[0]);
        cx->ii->store(i_na[0]);
    };

    DaqThread dt(cfg);
    CHECK(dt.start());

    // Let it run for 500 ms wall-clock.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    dt.stop();

    // Give the writer thread a moment to drain.
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    writer.stop();

    // --- assertions -----------------------------------------------------

    // No fatal error.
    if (!dt.ok()) {
        std::fprintf(stderr, "  dt.last_error: %s\n", dt.last_error().c_str());
    }
    CHECK(dt.ok());
    CHECK(writer.ok());
    CHECK(dt.done());

    // Disk rejects should be 0 in a healthy run.
    CHECK(dt.disk_rejects() == 0);

    // Scan count should be roughly sample_rate × 0.5 s = 2500. Allow a
    // generous tolerance because DAQMock uses wall-clock timing and the
    // CI host may be loaded.
    const uint64_t scans = dt.scans_acquired();
    const double expected = sample_rate * 0.5;
    std::fprintf(stderr, "  scans=%llu expected≈%.0f rt=%s\n",
        (unsigned long long)scans, expected,
        rt_priority::to_string(dt.rt_report().status));
    CHECK(scans > (uint64_t)(expected * 0.5));   // at least half
    CHECK(scans < (uint64_t)(expected * 3.0));   // and not absurd

    // Chunks processed > 0 and callback fired for each chunk.
    CHECK(dt.chunks_processed() > 0);
    CHECK((uint64_t)chunk_callbacks.load() == dt.chunks_processed());

    // Writer saw the same scan count.
    CHECK(writer.scans_written() == scans);

    // The disk file should have the right size.
    std::error_code ec;
    const auto bytes = std::filesystem::file_size(path, ec);
    CHECK(!ec);
    const uint64_t expected_bytes =
        scan_writer_format::data_offset_for(wcfg.scan_size) +
        scans * wcfg.scan_size * sizeof(double);
    CHECK(bytes == expected_bytes);

    // Read back the header and spot-check the first scan.
    std::ifstream f(path, std::ios::binary);
    CHECK(f.is_open());
    scan_writer_format::Header h{};
    f.read(reinterpret_cast<char*>(&h), sizeof(h));
    CHECK(h.magic == scan_writer_format::kMagic);
    CHECK(h.num_scans_written == scans);
    CHECK(h.scan_size == 3);

    // Callback last-values should be non-absurd (Vm near -60..+50 mV,
    // current bounded by clip range).
    CHECK(std::abs(last_vm.load()) < 200.0);
    CHECK(std::abs(last_i.load()) < 20.0);
}

static void daq_thread_stops_cleanly_without_writer() {
    auto net = make_test_network();
    DAQMock daq;
    daq.configureAI("MockDev1/ai0", 5000.0);
    daq.configureAO("MockDev1/ao0");

    DaqThread::Config cfg;
    cfg.daq = &daq;
    cfg.net = net.get();
    cfg.writer = nullptr;        // headless — no disk
    cfg.sample_rate_hz = 5000.0;
    cfg.num_ai_channels = 1;
    cfg.num_cells = 1;
    cfg.num_vdep_cells = 1;

    DaqThread dt(cfg);
    CHECK(dt.start());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    dt.stop();

    CHECK(dt.ok());
    CHECK(dt.done());
    CHECK(dt.scans_acquired() > 0);
}

static void daq_thread_rejects_bad_config() {
    auto net = make_test_network();
    DAQMock daq;

    DaqThread::Config cfg;
    cfg.daq = &daq;
    cfg.net = net.get();
    cfg.sample_rate_hz = 0.0;   // invalid
    cfg.num_ai_channels = 1;
    cfg.num_cells = 1;

    DaqThread dt(cfg);
    CHECK(!dt.start());
    CHECK(!dt.last_error().empty());
}

// ---------------------------------------------------------------------------
int main() {
    std::printf("NETrex DaqThread end-to-end tests\n");
    std::printf("=================================\n\n");

    run("daq_thread runs against mock",       daq_thread_runs_against_mock);
    run("stops cleanly without a writer",     daq_thread_stops_cleanly_without_writer);
    run("rejects bad config",                 daq_thread_rejects_bad_config);

    std::printf("\n");
    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    }
    std::printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
