#include "bloom_filter.h"
bloom_filter::bloom_filter(double prob_false_pos, long expected_num_el) : 
    num_el(0), 
    fill_num(0), 
    filter(get_optimal_size(prob_false_pos, expected_num_el)) {
    }

void bloom_filter::insert(const std::string& key) {
    hash(key);
    //hash_buf is 128 bit
    const uint64_t *ptr = (const uint64_t *) hash_buf;
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

bool bloom_filter::contains(const std::string& key) {
    hash(key);
    const uint64_t *ptr = (const uint64_t *) hash_buf;
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

size_t bloom_filter::size() {
    return num_el;
}

double bloom_filter::fill_ratio() {
    return ((double) filter.num_filled()) / filter.bits();
}

const bloom_filter bloom_filter::operator^(const bloom_filter& bf) {
    return *this;
}

void bloom_filter::hash(const std::string& key) {
    MurmurHash3_x64_128(key.data(), key.size(), SEED, (void *) hash_buf);
}

bool bloom_filter::insert_slice(int ind, uint64_t hash1, uint64_t hash2) {
    //double hashing
    // can use & (filter.bits() - 1)  if num_bits power of 2
    int slice = (hash1 + ind * hash2) % filter.bits();
    if (!filter[slice]) {
        filter.set(slice, 1);
        return 1;
    }
    return 0;
}

long bloom_filter::get_optimal_size(double prob_false_pos, long expected_num_el) {
    long optimal_size = ceil(-(expected_num_el * log(prob_false_pos)) / (LN2 * LN2));
    num_hashes = round(((double) optimal_size / expected_num_el) * LN2);
    return optimal_size;
}
