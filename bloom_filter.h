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
        const bloom_filter operator^(const bloom_filter& bf);

    private:
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
