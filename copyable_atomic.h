#ifndef COPYABLE_ATOMIC_H
#define COPYABLE_ATOMIC_H
#include <atomic>
#include <iostream>

template<class T>
class copyable_atomic : public std::atomic<T> {
    
    public:
        copyable_atomic() = default;

        copyable_atomic(T el) : std::atomic<T>(el) {
        
        }

        copyable_atomic(const copyable_atomic<T>& other) : copyable_atomic(other.load(std::memory_order_relaxed)) {

        }

        copyable_atomic& operator=(const copyable_atomic<T>& other) {
            this->store(other.load(std::memory_order_acquire), std::memory_order_release);
            return *this;
        }

};
#endif
