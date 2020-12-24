#ifndef ATOMIC_BITSET_H
#define ATOMIC_BITSET_H
#include <atomic>
#include <iostream>
#include "copyable_atomic.h"

class atomic_bitset {
    public:
        static constexpr unsigned int WORD_SIZE_BITS = 8 * sizeof(unsigned int); 
        //constructor not thread safe
        atomic_bitset(long bits);
        //custom buffer
        atomic_bitset(copyable_atomic<unsigned int> *buf, long bits); 

        atomic_bitset(const atomic_bitset& other);
        atomic_bitset& operator=(atomic_bitset other);
        atomic_bitset(atomic_bitset&& other);

        //number of bits 
        long bits() const;
        //number of bits == 1
        size_t num_filled();
        //set bit at ind to val
        bool set(size_t ind, bool val);
        //get bit at index ind
        inline bool get(size_t ind) {
            return bit_arr[get_byte(ind)].load(std::memory_order_relaxed) & get_mask(ind);
        }
        //equivalent to get
        bool operator[](size_t ind);
        
        //not thread safe

        ~atomic_bitset();

        friend void swap(atomic_bitset& first, atomic_bitset& second) {
            using std::swap;
            std::swap(first.num_bits, second.num_bits);
            std::swap(first.num_set, second.num_set);
            std::swap(first.bit_arr, second.bit_arr);
        }

    private:
        inline unsigned int get_byte(size_t ind) {
            return ind / WORD_SIZE_BITS;
        } 
        inline unsigned int get_mask(size_t ind) {
            //assume WORD_SIZE_BITS power of 2
            return 1 << (ind & (WORD_SIZE_BITS - 1));
            //return 1 << (ind % WORD_SIZE_BITS);
        }


        copyable_atomic<unsigned int> *bit_arr = 0;
        long num_bits;
        copyable_atomic<size_t>  num_set;
};

#endif
