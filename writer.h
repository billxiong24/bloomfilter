#ifndef WRITER_H
#define WRITER_H
#include <string>
#include "atomic_bitset.h"
class writer {
    public:
        virtual bool serialize(void *, size_t) = 0;
        virtual std::string deserialize(const std::string& key) = 0;
        virtual ~writer() {}
};
#endif

