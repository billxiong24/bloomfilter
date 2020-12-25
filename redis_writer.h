#ifndef REDIS_WRITER_H
#define REDIS_WRITER_H
#include "writer.h"
#include <string>
#include <hiredis/hiredis.h>
class redis_writer : public writer {


    public:
        redis_writer(const std::string& ip, int port);
        virtual bool serialize(void *, size_t);
        virtual std::string deserialize(const std::string& key);
        virtual ~redis_writer();

    private:
        redisContext* ctx = 0;
};


#endif
