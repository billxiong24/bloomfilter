#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include "atomic_bitset.h"
#include <bitset>
#include <iostream>
#include <math.h>
#include "murmur3.h"

class bloom_filter {
    public:
        bloom_filter(double prob_false_pos, long expected_num_el);
        void insert(const std::string& key);
        bool contains(const std::string& key);
        size_t size();
        double fill_ratio();

        bloom_filter operator |(const bloom_filter& other);
        bloom_filter operator &(const bloom_filter& other);
        bloom_filter operator ^(const bloom_filter& other);
        bloom_filter operator <<(const bloom_filter& other);
        bloom_filter operator >>(const bloom_filter& other);

        bloom_filter& operator |=(const bloom_filter& other);
        bloom_filter& operator &=(const bloom_filter& other);
        bloom_filter& operator ^=(const bloom_filter& other);
        bloom_filter& operator <<=(const bloom_filter& other);
        bloom_filter& operator >>=(const bloom_filter& other);

    private:
        inline void set_ind(
                atomic_bitset& filter, 
                const atomic_bitset& other_filter,
                std::function<bool(bool, bool)> fn) {

            size_t sz = filter.bits();
            size_t other_sz = other_filter.bits();

            for (int i = 0; i < sz; i++) {
                filter.set(i, fn(filter[i], other_filter[i]));
            }
        }


        void hash(const std::string& key);
        bool insert_slice(int ind, uint64_t hash1, uint64_t hash2); 
        long get_optimal_size(double prob_false_pos, long expected_num_el);

        static constexpr int HASH_BUF_SIZE = 4;
        static constexpr double LN2 = 0.69314718056;
        static constexpr uint32_t SEED = 0x1C948969;
 
        //align to 16 byte boundary
        uint32_t hash_buf alignas(HASH_BUF_SIZE) [HASH_BUF_SIZE];
        atomic_bitset filter;
        unsigned int num_hashes;
        size_t num_el, fill_num;
};

#endif
