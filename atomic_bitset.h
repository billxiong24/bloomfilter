#ifndef ATOMIC_BITSET_H
#define ATOMIC_BITSET_H
#include <atomic>
#include <iostream>

class atomic_bitset {
    public:
        static constexpr unsigned int WORD_SIZE_BITS = 8 * sizeof(unsigned int); 
        //constructor not thread safe
        atomic_bitset(long bits);
        //custom buffer
        atomic_bitset(std::atomic<unsigned int> *buf, long bits); 
        //number of bits 
        long bits() const;
        //number of bits == 1
        size_t num_filled();
        //set bit at ind to val
        bool set(size_t ind, bool val);
        //get bit at index ind
        inline bool get(size_t ind);
        //equivalent to get
        bool operator[](size_t ind);

        ~atomic_bitset();
    private:
        inline unsigned int get_byte(size_t ind); 
        inline unsigned int get_mask(size_t ind);

        std::atomic<unsigned int> *bit_arr = 0;
        long num_bits;
        std::atomic<size_t> num_set;
};

#endif
