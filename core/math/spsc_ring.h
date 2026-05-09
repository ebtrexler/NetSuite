// core/math/spsc_ring.h
//
// Single-producer / single-consumer drop-oldest ring buffer, intended for
// the DAQ → UI plotting path. The producer (DAQ thread) never waits; when
// the buffer is full, the oldest sample is overwritten. The consumer
// (UI thread) keeps its own monotonic cursor and snapshots "everything
// written since I last looked". If the consumer has fallen so far behind
// that data has been overwritten, the snapshot reports how many samples
// were lost and yields the most-recent window instead — this is exactly
// the "UI plotter catches up after an OS pause" behavior we want.
//
// T must be trivially copyable. Capacity is rounded up to a power of 2
// so that indexing is a cheap bitwise AND.
//
// This file is header-only and has no dependencies outside the C++17
// standard library, so it is safe to include from both netrex_core
// and the Qt UI.
//
// Part of the NETrex real-time DAQ pipeline. See Developer_Guide.md §4.3.
//
// E. Brady Trexler / hudsonvalleysci.com

#ifndef SPSC_RING_H
#define SPSC_RING_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <type_traits>

#if defined(__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 201603
#include <new>
namespace spsc_detail {
    constexpr std::size_t kCacheLine = std::hardware_destructive_interference_size;
}
#else
namespace spsc_detail {
    constexpr std::size_t kCacheLine = 64;
}
#endif

/// Single-producer single-consumer ring with drop-oldest semantics.
///
/// Usage:
/// @code
///   SpscRing<Scan> ring(/*capacity=*/1 << 17);   // ~131k scans
///   // --- producer (DAQ thread) ---
///   ring.push(scan);
///   // --- consumer (UI thread) ---
///   uint64_t cursor = 0;
///   Scan out[2048];
///   uint64_t lost = 0;
///   size_t n = ring.snapshot_since(cursor, out, 2048, &lost);
/// @endcode
template <typename T>
class SpscRing {
    static_assert(std::is_trivially_copyable<T>::value,
                  "SpscRing<T> requires trivially copyable T");

public:
    /// Constructs a ring with at least `capacity` slots. The actual
    /// capacity is rounded up to the next power of 2 (minimum 2).
    explicit SpscRing(std::size_t capacity)
        : m_cap(round_up_pow2(capacity < 2 ? 2 : capacity)),
          m_mask(m_cap - 1),
          m_slots(new Slot[m_cap])
    {
        // Slot's default ctor zero-initializes seq.
    }

    SpscRing(const SpscRing&) = delete;
    SpscRing& operator=(const SpscRing&) = delete;

    /// Producer: push a value. Always succeeds; overwrites oldest on full.
    /// Must be called from exactly one thread.
    void push(const T& value) {
        const uint64_t h = m_head.load(std::memory_order_relaxed);
        const std::size_t idx = static_cast<std::size_t>(h) & m_mask;

        // Invalidate the slot first so a concurrent reader that had
        // already latched the old seq value doesn't accept a torn read.
        // Any seq that isn't h+1 will fail the reader's post-check.
        m_slots[idx].seq.store(0, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_release);

        // Copy the payload, then publish the new seq.
        std::memcpy(&m_slots[idx].data, &value, sizeof(T));
        m_slots[idx].seq.store(h + 1, std::memory_order_release);

        // Advance head last so the consumer sees the seq update first.
        m_head.store(h + 1, std::memory_order_release);
    }

    /// Consumer: copy into `out` everything written since `cursor`, up to
    /// `max_out` items. On return, `cursor` is advanced by the number of
    /// items successfully copied. If the producer has lapped the
    /// consumer, `*overflow_lost` (if provided) is set to the number of
    /// items that were overwritten before we could read them, and
    /// `cursor` jumps forward to the oldest still-in-buffer item.
    ///
    /// Returns the number of items copied.
    ///
    /// Must be called from exactly one thread (the consumer).
    std::size_t snapshot_since(uint64_t& cursor,
                               T* out,
                               std::size_t max_out,
                               uint64_t* overflow_lost = nullptr) const {
        if (overflow_lost) *overflow_lost = 0;

        const uint64_t head = m_head.load(std::memory_order_acquire);
        if (cursor > head) {
            // Defensive: the consumer supplied a cursor from the future.
            cursor = head;
            return 0;
        }

        uint64_t available = head - cursor;
        const uint64_t cap64 = static_cast<uint64_t>(m_cap);

        // If we've been lapped, jump forward to the oldest item still
        // present in the ring. Report the gap so the caller can log it.
        if (available > cap64) {
            const uint64_t lost = available - cap64;
            if (overflow_lost) *overflow_lost = lost;
            cursor = head - cap64;
            available = cap64;
        }

        std::size_t to_copy =
            available < static_cast<uint64_t>(max_out)
                ? static_cast<std::size_t>(available)
                : max_out;

        std::size_t copied = 0;
        for (std::size_t i = 0; i < to_copy; ++i) {
            const uint64_t pos = cursor + i;
            const std::size_t idx = static_cast<std::size_t>(pos) & m_mask;

            // Two-read seq check brackets a torn-read window. If the
            // producer overwrites this slot between the pre- and post-
            // reads, bail out of this snapshot; the consumer will catch
            // up on the next call.
            const uint64_t seq_pre =
                m_slots[idx].seq.load(std::memory_order_acquire);
            if (seq_pre != pos + 1) break;

            T value;
            std::memcpy(&value, &m_slots[idx].data, sizeof(T));

            std::atomic_thread_fence(std::memory_order_acquire);
            const uint64_t seq_post =
                m_slots[idx].seq.load(std::memory_order_acquire);
            if (seq_post != pos + 1) break;

            out[copied++] = value;
        }

        cursor += copied;
        return copied;
    }

    /// Total number of items ever pushed. Monotonic; safe to call from
    /// either thread.
    uint64_t total_pushed() const {
        return m_head.load(std::memory_order_acquire);
    }

    /// Ring capacity (always a power of 2).
    std::size_t capacity() const { return m_cap; }

private:
    struct Slot {
        // seq = (producer-position + 1) for the last completed write.
        // seq = 0 marks "empty / being written". The consumer accepts a
        // slot only when seq == pos + 1 both before and after the read.
        std::atomic<uint64_t> seq;
        T data;

        Slot() : seq(0) {
            // Zero the payload to keep tools like valgrind quiet; it's a
            // POD so the cost is trivial.
            std::memset(&data, 0, sizeof(T));
        }
    };

    static std::size_t round_up_pow2(std::size_t n) {
        std::size_t p = 1;
        while (p < n) p <<= 1;
        return p;
    }

    const std::size_t m_cap;
    const std::size_t m_mask;
    std::unique_ptr<Slot[]> m_slots;

    // Keep producer state on its own cache line to avoid false sharing
    // with the consumer's cursor (which lives in the caller's frame).
    alignas(spsc_detail::kCacheLine) std::atomic<uint64_t> m_head{0};
    char m_pad[spsc_detail::kCacheLine - sizeof(std::atomic<uint64_t>)]{};
};

#endif // SPSC_RING_H
