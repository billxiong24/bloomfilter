#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include "atomic_bitset.h"
#include <bitset>
#include <openssl/md5.h>
#include <iostream>
#include <math.h>

class bloom_filter {
    public:
        bloom_filter(double prob_false_pos, long expected_num_el) : 
            num_el(0), 
            fill_num(0), 
            filter(get_optimal_size(prob_false_pos, expected_num_el)) {

        }

        bloom_filter(const bloom_filter& bf) : 
            filter(bf.filter.bits()), 
            num_hashes(bf.num_hashes), 
            num_el(bf.num_el), 
            fill_num(bf.fill_num) {

        }

        void insert(const std::string& key) {
            hash(key);
            //md5_buf is 128 bit
            const uint64_t *ptr = (const uint64_t *) md5_buf;
            const uint64_t hash1 = ptr[0];
            const uint64_t hash2 = ptr[1];
            int num_set = 0;
            for (int i = 0; i < num_hashes; i++) {
                if (insert_slice(i, hash1, hash2))
                    ++num_set;
            }
            if (num_set > 0)
                ++num_el;
        }

        bool contains(const std::string& key) {
            hash(key);
            const uint64_t *ptr = (const uint64_t *) md5_buf;
            const uint64_t hash1 = ptr[0];
            const uint64_t hash2 = ptr[1];
            int num_set = 0;
            for (int i = 0; i < num_hashes; i++) {
                int slice = (hash1 + i * hash2) % filter.bits();
                if (!filter[slice])
                    return 0;
            }
            return 1;
        }

        size_t size() {
            return num_el;
        }

        double fill_ratio() {
            return ((double) filter.num_filled()) / filter.bits();
        }

    private:
        void hash(const std::string& key) {
            MD5((const unsigned char*) key.c_str(), key.size(), md5_buf);
        }

        bool insert_slice(int ind, uint64_t hash1, uint64_t hash2) {
            //double hashing
            // can use & (filter.bits() - 1)  if num_bits power of 2
            int slice = (hash1 + ind * hash2) % filter.bits();
            if (!filter[slice]) {
                filter.set(slice, 1);
                return 1;
            }
            return 0;
        }

        long get_optimal_size(double prob_false_pos, long expected_num_el) {
            long optimal_size = ceil(-(expected_num_el * log(prob_false_pos)) / (LN2 * LN2));
            num_hashes = round(((double) optimal_size / expected_num_el) * LN2);
            return optimal_size;
        }

        static constexpr int MD5_SIZE = 16;
        static constexpr double LN2 = 0.69314718056;
        //align to 16 byte boundary
        unsigned char md5_buf alignas(MD5_SIZE) [MD5_SIZE];
        //std::bitset<NUM_BITS>nonatomic_filter;
        atomic_bitset filter;

        unsigned int num_hashes;
        size_t num_el, fill_num;
};

#endif
