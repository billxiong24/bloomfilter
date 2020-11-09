#ifndef ATOMIC_BITSET_H
#define ATOMIC_BITSET_H
#include <atomic>
#include <iostream>

class atomic_bitset {
    public:
        static constexpr unsigned int WORD_SIZE_BITS = 8 * sizeof(unsigned int); 
        atomic_bitset(long bits) : num_set{0}, num_bits{bits}, bit_arr{new std::atomic<unsigned int>[bits/8 + 1]} {
            for (int i = 0; i < bits/8 + 1; i++) {
                bit_arr[i] = 0;
            }
        }

        long bits() {
            return num_bits;
        }

        size_t num_filled() {
            return num_set.load(std::memory_order_relaxed);
        }

        bool set(size_t ind, bool val) {

            unsigned int mask = get_mask(ind);
            unsigned int byte = get_byte(ind);
            unsigned int curr_byte = bit_arr[byte].load(std::memory_order_relaxed);
            unsigned int copy = curr_byte;

            if (val) {
                if (curr_byte & mask) 
                    return 0;
                //replace bit_arr[byte] with 1 iff bit_arr[byte] == curr_byte. Only if true, then ++num_set. 
                std::atomic_compare_exchange_weak(&bit_arr[byte], &curr_byte, (unsigned int) (curr_byte | mask));
                if (curr_byte == copy)
                    ++num_set;
            }
            else {
                if ((curr_byte & mask) == 0) 
                    return 0;
                //replace bit_arr[byte] with 0 iff bit_arr[byte] == curr_byte. Only if true, then --num_set. 
                std::atomic_compare_exchange_weak(&bit_arr[byte], &curr_byte, (unsigned int) (curr_byte & ~mask));
                if (curr_byte == copy)
                    --num_set;
            }

            return curr_byte == copy;
        }

        inline bool get(size_t ind) {
            return bit_arr[get_byte(ind)].load(std::memory_order_relaxed) & get_mask(ind);
        }

        bool operator[](size_t ind) {
            return get(ind);
        }

        ~atomic_bitset() {
            if (bit_arr)
                delete[] bit_arr;
        }

    private:

        inline unsigned int get_byte(size_t ind) {
            return ind / WORD_SIZE_BITS;
        }
        inline unsigned int get_mask(size_t ind) {
            //assume WORD_SIZE_BITS power of 2
            return 1 << (ind & (WORD_SIZE_BITS - 1));
            //return 1 << (ind % WORD_SIZE_BITS) ;
        }

        std::atomic<unsigned int> *bit_arr = 0;
        long num_bits;
        std::atomic<size_t> num_set;
};

#endif
