#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <vector>
#include <cstddef>
#include <cassert>

// Fixed-capacity circular buffer for real-time data.
// Not thread-safe — caller must synchronize if used across threads.
template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity = 0)
        : m_data(capacity), m_capacity(capacity), m_head(0), m_size(0) {}

    void resize(size_t capacity) {
        m_data.resize(capacity);
        m_capacity = capacity;
        m_head = 0;
        m_size = 0;
    }

    void push(const T &value) {
        if (m_capacity == 0) return;
        m_data[m_head] = value;
        m_head = (m_head + 1) % m_capacity;
        if (m_size < m_capacity) ++m_size;
    }

    // Access element by logical index (0 = oldest)
    const T& operator[](size_t i) const {
        assert(i < m_size);
        size_t idx = (m_head + m_capacity - m_size + i) % m_capacity;
        return m_data[idx];
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

    void clear() { m_head = 0; m_size = 0; }

    // Oldest and newest elements
    const T& front() const { return (*this)[0]; }
    const T& back() const { return (*this)[m_size - 1]; }

private:
    std::vector<T> m_data;
    size_t m_capacity;
    size_t m_head;  // next write position
    size_t m_size;
};

#endif // RING_BUFFER_H
