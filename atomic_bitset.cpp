#include "atomic_bitset.h"
#define BITS_IN_BYTE 8

//constructor not thread safe
atomic_bitset::atomic_bitset(long bits) : 
    num_set{0}, 
    num_bits{bits}, 
    bit_arr{new copyable_atomic<unsigned int>[bits/BITS_IN_BYTE/sizeof(unsigned int) + 1]} {

    for (int i = 0; i < bits/BITS_IN_BYTE/sizeof(unsigned int) + 1; i++) {
        bit_arr[i] = 0;
    }
}

atomic_bitset::atomic_bitset(copyable_atomic<unsigned int> *buf, long bits) : num_set{0}, num_bits{bits}, bit_arr{buf} {}


atomic_bitset::atomic_bitset(atomic_bitset& other) : atomic_bitset(other.num_bits) {
    num_set = other.num_set;
    for (int i = 0; i < other.num_bits/BITS_IN_BYTE/sizeof(unsigned int) + 1; i++) {
        bit_arr[i] = other.bit_arr[i];
    }
}

atomic_bitset::atomic_bitset(atomic_bitset&& other) :atomic_bitset(other.num_bits) {
    swap(*this, other);
}

atomic_bitset& atomic_bitset::operator=(atomic_bitset other) {
    swap(*this, other);
    return *this;
}


long atomic_bitset::bits() const {
    return num_bits;
}

size_t atomic_bitset::num_filled() {
    return num_set.load(std::memory_order_relaxed);
}

bool atomic_bitset::set(size_t ind, bool val) {

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

bool atomic_bitset::operator[](size_t ind) {
    return get(ind);
}

atomic_bitset::~atomic_bitset() {
    if (bit_arr)
        delete[] bit_arr;
}
