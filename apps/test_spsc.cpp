// apps/test_spsc.cpp
//
// Unit tests for the SPSC ring buffer and SPSC queue that back the
// real-time DAQ pipeline. Plain C++17 with std::thread; no external test
// framework. Returns 0 on success, nonzero on first failure.
//
// Run:   ./test_spsc
//
// These tests are designed to be fast enough (< a few seconds on a laptop)
// to keep in a pre-commit / CI smoke suite.

#include "spsc_ring.h"
#include "spsc_queue.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

// ---------------------------------------------------------------------------
// Tiny assertion macros. On failure, print location and return from main.
// ---------------------------------------------------------------------------
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
    if (_a != _e) {                                                          \
        std::fprintf(stderr, "  [FAIL] %s:%d: %s != %s (got %lld vs %lld)\n",\
                     __FILE__, __LINE__, #actual, #expected,                 \
                     static_cast<long long>(_a),                             \
                     static_cast<long long>(_e));                            \
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

// ===========================================================================
// SpscRing tests
// ===========================================================================

// ST = single-threaded basic sanity
static void ring_single_thread_roundtrip() {
    SpscRing<int> ring(16);
    CHECK_EQ(ring.capacity(), (std::size_t)16);

    for (int i = 1; i <= 10; ++i) ring.push(i);

    uint64_t cursor = 0;
    uint64_t lost = 0;
    int out[32] = {};
    std::size_t n = ring.snapshot_since(cursor, out, 32, &lost);

    CHECK_EQ(n, (std::size_t)10);
    CHECK_EQ(lost, (uint64_t)0);
    CHECK_EQ(cursor, (uint64_t)10);
    for (int i = 0; i < 10; ++i) CHECK_EQ(out[i], i + 1);

    // A follow-up snapshot with no new pushes should return 0.
    n = ring.snapshot_since(cursor, out, 32, &lost);
    CHECK_EQ(n, (std::size_t)0);
    CHECK_EQ(lost, (uint64_t)0);
}

// ST = drop-oldest behavior when producer laps consumer
static void ring_single_thread_overflow() {
    SpscRing<int> ring(8);  // capacity = 8

    // Push 20 items; only the last 8 survive.
    for (int i = 1; i <= 20; ++i) ring.push(i);

    uint64_t cursor = 0;
    uint64_t lost = 0;
    int out[32] = {};
    std::size_t n = ring.snapshot_since(cursor, out, 32, &lost);

    // We should get exactly 8 items (capacity), they should be the most
    // recent 8 (13..20), and the overflow report should say we lost 12.
    CHECK_EQ(n, (std::size_t)8);
    CHECK_EQ(lost, (uint64_t)12);
    CHECK_EQ(cursor, (uint64_t)20);
    for (int i = 0; i < 8; ++i) CHECK_EQ(out[i], 13 + i);
}

// ST = snapshot with max_out smaller than available items
static void ring_single_thread_partial_snapshot() {
    SpscRing<int> ring(16);
    for (int i = 1; i <= 10; ++i) ring.push(i);

    uint64_t cursor = 0;
    int out[4] = {};
    std::size_t n = ring.snapshot_since(cursor, out, 4);
    CHECK_EQ(n, (std::size_t)4);
    CHECK_EQ(cursor, (uint64_t)4);
    for (int i = 0; i < 4; ++i) CHECK_EQ(out[i], i + 1);

    // Second call picks up where we left off.
    n = ring.snapshot_since(cursor, out, 4);
    CHECK_EQ(n, (std::size_t)4);
    CHECK_EQ(cursor, (uint64_t)8);
    for (int i = 0; i < 4; ++i) CHECK_EQ(out[i], 5 + i);
}

// MT = producer runs hot; consumer snapshots a subset; the union of all
// snapshots + reported overflow must equal what the producer pushed.
static void ring_mt_producer_faster_than_consumer() {
    constexpr std::size_t kCap = 1 << 12;  // 4096 slots
    constexpr uint64_t kToProduce = 500'000;

    SpscRing<uint64_t> ring(kCap);
    std::atomic<bool> done{false};

    std::thread producer([&]() {
        for (uint64_t i = 0; i < kToProduce; ++i) {
            ring.push(i);
            // Keep the producer slightly slower than memcpy speed so the
            // consumer has a fighting chance of keeping up sometimes.
            if ((i & 0xFF) == 0) std::this_thread::yield();
        }
        done.store(true, std::memory_order_release);
    });

    std::vector<uint64_t> seen;
    seen.reserve(kToProduce);
    uint64_t cursor = 0;
    uint64_t total_lost = 0;
    std::vector<uint64_t> batch(kCap);

    // Consumer: snapshot in a tight loop but do no real work (bad UI) —
    // we still expect every received item to be strictly monotonic, no
    // duplicates, and lost + received == produced at the end.
    while (!done.load(std::memory_order_acquire) || ring.total_pushed() > cursor) {
        uint64_t lost = 0;
        std::size_t n = ring.snapshot_since(cursor, batch.data(), batch.size(), &lost);
        total_lost += lost;
        for (std::size_t i = 0; i < n; ++i) seen.push_back(batch[i]);
        if (n == 0) std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    producer.join();

    // All items we received must be strictly increasing.
    for (std::size_t i = 1; i < seen.size(); ++i) {
        CHECK(seen[i] > seen[i - 1]);
    }

    // Accounting identity: received + lost == produced.
    CHECK_EQ((uint64_t)seen.size() + total_lost, kToProduce);
    CHECK_EQ(ring.total_pushed(), kToProduce);
}

// MT = simulated UI stall. Consumer sleeps for a while after a few early
// snapshots; when it resumes, overflow should be reported and the rest
// of the data should still be received in order.
static void ring_mt_consumer_stall() {
    constexpr std::size_t kCap = 1 << 10;  // small on purpose
    SpscRing<uint64_t> ring(kCap);

    std::atomic<bool> producer_started{false};
    std::atomic<bool> stop{false};
    std::atomic<uint64_t> produced{0};

    std::thread producer([&]() {
        producer_started.store(true);
        uint64_t i = 0;
        while (!stop.load()) {
            ring.push(i++);
            // Aim for ~2 items per microsecond → plenty of overflow
            // potential during the stall.
            if ((i & 0x3FF) == 0) std::this_thread::yield();
        }
        produced.store(i);
    });

    while (!producer_started.load()) std::this_thread::yield();

    uint64_t cursor = 0;
    uint64_t total_lost = 0;
    std::vector<uint64_t> batch(kCap * 4);
    std::vector<uint64_t> seen;

    // One normal snapshot.
    {
        uint64_t lost = 0;
        std::size_t n = ring.snapshot_since(cursor, batch.data(), batch.size(), &lost);
        total_lost += lost;
        for (std::size_t i = 0; i < n; ++i) seen.push_back(batch[i]);
    }

    // Stall — simulate a 50 ms OS pause in the UI thread.
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Drain whatever is there right now. We expect overflow > 0.
    {
        uint64_t lost = 0;
        std::size_t n = ring.snapshot_since(cursor, batch.data(), batch.size(), &lost);
        total_lost += lost;
        for (std::size_t i = 0; i < n; ++i) seen.push_back(batch[i]);
        CHECK(lost > 0);  // the whole point of this test
    }

    // Stop the producer and drain the tail.
    stop.store(true);
    producer.join();

    while (cursor < produced.load()) {
        uint64_t lost = 0;
        std::size_t n = ring.snapshot_since(cursor, batch.data(), batch.size(), &lost);
        total_lost += lost;
        for (std::size_t i = 0; i < n; ++i) seen.push_back(batch[i]);
        if (n == 0) break;
    }

    // Monotonic: even after the stall, the received items must still be
    // strictly increasing.
    for (std::size_t i = 1; i < seen.size(); ++i) {
        CHECK(seen[i] > seen[i - 1]);
    }

    // Conservation: received + overflow == produced.
    CHECK_EQ((uint64_t)seen.size() + total_lost, produced.load());
}

// ===========================================================================
// SpscQueue tests
// ===========================================================================

static void queue_single_thread_fifo() {
    SpscQueue<int> q(8);
    // capacity() is "at least what was asked for" (the backing store is
    // rounded up to a power of 2 internally).
    CHECK(q.capacity() >= (std::size_t)8);
    CHECK(q.empty());

    for (int i = 1; i <= 5; ++i) CHECK(q.try_push(i));

    CHECK(!q.empty());
    CHECK_EQ(q.size_approx(), (std::size_t)5);

    int out = 0;
    for (int i = 1; i <= 5; ++i) {
        CHECK(q.try_pop(out));
        CHECK_EQ(out, i);
    }
    CHECK(q.empty());
    CHECK(!q.try_pop(out));
}

static void queue_single_thread_full_returns_false() {
    SpscQueue<int> q(4);

    // capacity() should be at least 4; we'll push exactly capacity() items.
    std::size_t cap = q.capacity();
    for (std::size_t i = 0; i < cap; ++i) CHECK(q.try_push(static_cast<int>(i)));

    // Next push must fail — no silent drop.
    CHECK(!q.try_push(999));
    CHECK_EQ(q.size_approx(), cap);

    // After one pop, a push must succeed again.
    int out = 0;
    CHECK(q.try_pop(out));
    CHECK_EQ(out, 0);
    CHECK(q.try_push(999));
}

// Move-only payload: ensure the queue really moves rather than copies.
// std::vector<double> is move-only-effective: moving leaves the source
// empty, copying does not.
static void queue_move_only_payload() {
    SpscQueue<std::vector<double>> q(8);
    std::vector<double> big(1000, 3.14);
    double *orig_data = big.data();

    CHECK(q.try_push(std::move(big)));
    CHECK(big.empty());  // proof we moved, not copied

    std::vector<double> out;
    CHECK(q.try_pop(out));
    CHECK_EQ(out.size(), (std::size_t)1000);
    CHECK(out.data() == orig_data);  // same buffer — confirms move, not copy
    CHECK_EQ(out.front(), 3.14);
}

// MT = hot producer + slow-ish consumer. Every item must arrive exactly
// once, in order, with no losses (guaranteed delivery).
static void queue_mt_no_loss() {
    constexpr std::size_t kCap = 1024;
    constexpr uint64_t kToProduce = 250'000;

    SpscQueue<uint64_t> q(kCap);

    std::thread producer([&]() {
        for (uint64_t i = 0; i < kToProduce; ++i) {
            while (!q.try_push(i)) {
                // Full → spin briefly. In the real pipeline, a persistently
                // full queue is the abort signal, but here the consumer
                // will always catch up.
                std::this_thread::yield();
            }
        }
    });

    std::vector<uint64_t> received;
    received.reserve(kToProduce);
    uint64_t next_expected = 0;
    uint64_t v = 0;

    while (received.size() < kToProduce) {
        if (q.try_pop(v)) {
            if (v != next_expected) {
                std::fprintf(stderr,
                    "  [FAIL] queue_mt_no_loss: expected %llu got %llu\n",
                    (unsigned long long)next_expected,
                    (unsigned long long)v);
                ++g_failures;
                producer.join();
                return;
            }
            received.push_back(v);
            ++next_expected;
        } else {
            std::this_thread::yield();
        }
    }

    producer.join();
    CHECK_EQ(received.size(), (std::size_t)kToProduce);
}

// MT + real-world shape: producer pushes chunks (vectors of doubles),
// consumer drains and verifies sum. Simulates DAQ → disk-writer.
static void queue_mt_chunked_payload() {
    constexpr std::size_t kCap = 32;            // small, so producer blocks a lot
    constexpr int kNumChunks = 200;
    constexpr std::size_t kChunkSize = 512;

    SpscQueue<std::vector<double>> q(kCap);
    std::atomic<int> chunks_pushed{0};

    std::thread producer([&]() {
        for (int c = 0; c < kNumChunks; ++c) {
            std::vector<double> chunk(kChunkSize, static_cast<double>(c));
            while (!q.try_push(std::move(chunk))) {
                std::this_thread::yield();
            }
            chunks_pushed.fetch_add(1);
        }
    });

    std::vector<double> chunk;
    int chunks_received = 0;
    double total_sum = 0.0;

    while (chunks_received < kNumChunks) {
        if (q.try_pop(chunk)) {
            CHECK_EQ(chunk.size(), kChunkSize);
            // Every element in chunk #c should equal c.
            double expected = chunk.front();
            for (double d : chunk) {
                if (d != expected) {
                    std::fprintf(stderr, "  [FAIL] chunk content torn\n");
                    ++g_failures;
                    producer.join();
                    return;
                }
            }
            total_sum += expected * kChunkSize;
            ++chunks_received;
        } else {
            std::this_thread::yield();
        }
    }

    producer.join();

    // Expected sum: Σ c=0..199 of (c * 512) = 512 * 199 * 200 / 2
    double expected_sum = static_cast<double>(kChunkSize) *
                          (kNumChunks - 1) * kNumChunks / 2.0;
    CHECK_EQ(total_sum, expected_sum);
    CHECK(q.empty());
}

// ===========================================================================
// Main
// ===========================================================================

int main() {
    std::printf("NETrex SPSC data-structure tests\n");
    std::printf("================================\n\n");

    run("ring: single-thread roundtrip",     ring_single_thread_roundtrip);
    run("ring: single-thread overflow",      ring_single_thread_overflow);
    run("ring: single-thread partial snap",  ring_single_thread_partial_snapshot);
    run("ring: MT producer faster",          ring_mt_producer_faster_than_consumer);
    run("ring: MT consumer stall",           ring_mt_consumer_stall);

    run("queue: single-thread FIFO",         queue_single_thread_fifo);
    run("queue: single-thread full returns false",
                                             queue_single_thread_full_returns_false);
    run("queue: move-only payload",          queue_move_only_payload);
    run("queue: MT no loss",                 queue_mt_no_loss);
    run("queue: MT chunked payload",         queue_mt_chunked_payload);

    std::printf("\n");
    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("%d test(s) FAILED.\n", g_failures);
        return 1;
    }
}
