#ifndef SCALABLE_BLOOM_FILTER_H
#define SCALABLE_BLOOM_FILTER_H
#include "bloom_filter.h"
#include <vector>
class scalable_bloom_filter {

    public:
        scalable_bloom_filter() {

        }

    private:
        std::vector<bloom_filter> bloom_filters;

};
#endif
