#include "redis_writer.h"
#include <string.h>
redis_writer::redis_writer(const std::string& ip, int port) : ctx(redisConnect(ip.c_str(), port)) {
    if(!ctx) {
        puts("bad context");
    }
    else if(ctx->err) {
        puts(ctx->errstr);
    }
}

bool redis_writer::serialize(void *buf, size_t len) {
    redisCommand(ctx, "DEL key1");
    int *tmp = (int *) buf;
    std::string st;
    for (int i = 0; i < len/4; i++) {
        st += std::to_string(tmp[i]) + " ";
    }
    st[st.size() - 1] = '\0';

    std::string fmt = "RPUSH %s " + st;
    redisReply *reply = static_cast<redisReply *>(redisCommand(ctx, fmt.c_str(), "key1"));
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(ctx, "LRANGE key1 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int i = 0; i < reply->elements; i++) {
            std::cout << reply->element[i]->str << std::endl;
            
        }
    }

    freeReplyObject(reply);
    std::cout << "done" << std::endl;
    return false;
}

std::string redis_writer::deserialize(const std::string& key) {
    return "";
}

redis_writer::~redis_writer() {
    if (ctx)
        redisFree(ctx);
}

