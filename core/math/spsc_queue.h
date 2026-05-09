// core/math/spsc_queue.h
//
// Single-producer / single-consumer bounded FIFO queue. Unlike SpscRing
// (which drops oldest on overflow), SpscQueue never drops: `try_push`
// returns false when the queue is full, so the caller can choose its own
// back-pressure policy. This is what the DAQ → disk-writer path uses:
// losing an AI sample silently is unacceptable, so a full queue means
// "the disk can't keep up" and the run should abort rather than lose
// data.
//
// T may be move-only (e.g. std::unique_ptr, std::vector<double>). The
// queue takes ownership on push and returns ownership on pop.
//
// Capacity is rounded up to a power of 2. One slot is always kept
// reserved so that `head == tail` unambiguously means "empty" and
// `head - tail == capacity` means "full", without tracking size
// separately. (The capacity reported by `capacity()` is the usable
// capacity, not the internal slot count.)
//
// Part of the NETrex real-time DAQ pipeline. See Developer_Guide.md §4.5.
//
// E. Brady Trexler / hudsonvalleysci.com

#ifndef SPSC_QUEUE_H
#define SPSC_QUEUE_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

#if defined(__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 201603
#include <new>
namespace spsc_queue_detail {
    constexpr std::size_t kCacheLine = std::hardware_destructive_interference_size;
}
#else
namespace spsc_queue_detail {
    constexpr std::size_t kCacheLine = 64;
}
#endif

/// Lock-free single-producer single-consumer bounded FIFO queue.
///
/// Usage:
/// @code
///   SpscQueue<Chunk> q(/*capacity=*/256);
///   // --- producer (DAQ thread) ---
///   if (!q.try_push(std::move(chunk))) {
///       // queue full → disk writer is behind. Abort the run.
///   }
///   // --- consumer (writer thread) ---
///   Chunk out;
///   while (q.try_pop(out)) {
///       write_to_disk(out);
///   }
/// @endcode
template <typename T>
class SpscQueue {
public:
    /// Constructs a queue with usable capacity of at least `capacity`
    /// items (rounded up to pow2 − 1 internally; minimum 1).
    explicit SpscQueue(std::size_t capacity) {
        // Need one extra slot so head/tail can distinguish empty vs full
        // without a separate size counter.
        std::size_t internal = capacity < 1 ? 2 : capacity + 1;
        m_cap = round_up_pow2(internal);
        m_mask = m_cap - 1;
        // Use unique_ptr<T[]> because T might be atomic-bearing or
        // otherwise not copy-assignable via std::vector resize.
        m_slots.reset(new T[m_cap]());
    }

    SpscQueue(const SpscQueue&) = delete;
    SpscQueue& operator=(const SpscQueue&) = delete;

    /// Producer: push by copy. Returns false if full.
    bool try_push(const T& value) {
        const std::size_t h = m_head.load(std::memory_order_relaxed);
        const std::size_t t = m_tail.load(std::memory_order_acquire);
        if (h - t >= usable_capacity()) return false;

        m_slots[h & m_mask] = value;
        m_head.store(h + 1, std::memory_order_release);
        return true;
    }

    /// Producer: push by move. Returns false if full; the moved-from
    /// state of `value` is preserved (nothing was transferred) so the
    /// caller can retry.
    bool try_push(T&& value) {
        const std::size_t h = m_head.load(std::memory_order_relaxed);
        const std::size_t t = m_tail.load(std::memory_order_acquire);
        if (h - t >= usable_capacity()) return false;

        m_slots[h & m_mask] = std::move(value);
        m_head.store(h + 1, std::memory_order_release);
        return true;
    }

    /// Consumer: pop the oldest item into `out`. Returns false if empty.
    bool try_pop(T& out) {
        const std::size_t t = m_tail.load(std::memory_order_relaxed);
        const std::size_t h = m_head.load(std::memory_order_acquire);
        if (t == h) return false;

        out = std::move(m_slots[t & m_mask]);
        // Reset the slot so held resources (e.g. vector buffers) are
        // released promptly even if the queue never wraps back to this
        // slot.
        m_slots[t & m_mask] = T();
        m_tail.store(t + 1, std::memory_order_release);
        return true;
    }

    /// Approximate number of items currently in the queue. Safe to call
    /// from either thread; value is inherently racy.
    std::size_t size_approx() const {
        const std::size_t h = m_head.load(std::memory_order_acquire);
        const std::size_t t = m_tail.load(std::memory_order_acquire);
        return h - t;
    }

    bool empty() const {
        return m_head.load(std::memory_order_acquire) ==
               m_tail.load(std::memory_order_acquire);
    }

    /// Maximum number of items the queue can hold.
    std::size_t capacity() const { return usable_capacity(); }

private:
    std::size_t usable_capacity() const { return m_cap - 1; }

    static std::size_t round_up_pow2(std::size_t n) {
        std::size_t p = 1;
        while (p < n) p <<= 1;
        return p;
    }

    std::size_t m_cap   = 0;
    std::size_t m_mask  = 0;
    std::unique_ptr<T[]> m_slots;

    alignas(spsc_queue_detail::kCacheLine) std::atomic<std::size_t> m_head{0};
    char m_pad1[spsc_queue_detail::kCacheLine - sizeof(std::atomic<std::size_t>)]{};
    alignas(spsc_queue_detail::kCacheLine) std::atomic<std::size_t> m_tail{0};
    char m_pad2[spsc_queue_detail::kCacheLine - sizeof(std::atomic<std::size_t>)]{};
};

#endif // SPSC_QUEUE_H
