#ifndef COPYABLE_ATOMIC_H
#define COPYABLE_ATOMIC_H
#include <atomic>
#include <iostream>

template<typename T>
class copyable_atomic : public std::atomic<T> {
    public:
        copyable_atomic() = default;
        copyable_atomic(T obj) : std::atomic<T>(obj) {}

        copyable_atomic(const copyable_atomic& c) : std::atomic<T>(c.load(std::memory_order_relaxed)) {}

        copyable_atomic& operator=(const copyable_atomic<T>& c) {
            this->store(c.load(std::memory_order_acquire), std::memory_order_release);
            return *this;
        }

};

#endif
