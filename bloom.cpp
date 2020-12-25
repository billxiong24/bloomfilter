#include <iostream>
#include <type_traits>
#include <bitset>
#include <stdio.h>
#include "bloom_filter.h"
#include "atomic_bitset.h"
#include "murmur3.h"
#include "redis_writer.h"
#include <vector>
#include <thread>
#include "copyable_atomic.h"

int main(void) {
    redis_writer wt("127.0.0.1", 6379);

    bloom_filter bf1(0.01, 1000);
    bloom_filter bf4(0.01, 1000);
    bf1.insert("words");
    bf1 ^= bf4;
    //bf1.insert("wtaoi");
    //bf1.insert("teen");
    //bf4.insert("nextttt");
    std::cout << bf1.contains("words") << std::endl;

    //bf1 |= bf4;
    if(true)
        return 1;


    //wt.serialize(bs1.bit_arr, bytes);
    bloom_filter bf(0.01, 1000);
    bf.insert("wat");
    bf.insert("wat2");
    bf.insert("hello");
    bf.insert("dsfoier");
    bf.insert("2340x9xw");
    bf.insert("adfoiww3");
    bf.insert("azxkjjww");
    bf.insert("hellowrold");
    bf.insert("wateroisawtoi");
    bf.insert("worldlwld");
    bf.insert("aodfierddd");
    bf.insert("aowiercdidoiioo");
    bf.insert("222wwxoxoisjjdls");
    bf.insert("ssiduaooowrqwe");
    bf.insert("a");
    bf.insert("av");
    bf.insert("23edi");
    bf.insert("dddfkk");
    bf.insert("eeeq");
    bf.insert("yyy99");
    bf.insert("eeeqood");
    bf.insert("sdoieee");
    bf.insert("d993jss");
    bf.insert("heldoa");
    bf.insert("ssdoo100");
    bf.insert("12e9ed8suji");
    std::cout << bf.contains("wer9xcc") << std::endl;
    std::cout << bf.contains("yyy99") << std::endl;
    std::cout << bf.contains("hello") << std::endl;
    std::cout << bf.fill_ratio() << std::endl;
    bloom_filter bf2(bf);
    std::cout << "---test---" << std::endl;
    std::cout << bf2.contains("yyy99")<< std::endl;
    bf2.insert("55555544444");
    std::cout << bf2.contains("55555544444") << std::endl;
    std::cout << bf.contains("55555544444") << std::endl;
    if(1) return 0;


    atomic_bitset bs(64);
    auto lamb = [&bs]() {
        for (int i = 0; i < 1000; i++) {
            bs.set(3, 1);
            bs.set(5, 1);
            bs.set(25, 1);
        }
    };

    auto lamb2 = [&bs]() {
        for (int i = 0; i < 1000; i++) {
            bs.set(25, 0);
            bs.set(3, 0);
            bs.set(5, 0);
        }
    };
    std::thread arr[5];
    std::thread arr2[5];
    
    for (int i = 0; i < 5; i++) {
        arr2[i] = std::thread(lamb2);
    }

    for (int i = 0; i < 5; i++) {
        arr[i] = std::thread(lamb);
    }

    for (int i = 0; i < 5; i++) {
        arr2[i].join();
    }

    for (int i = 0; i < 5; i++) {
        arr[i].join();
    }


    for (int i = 0; i < 64; i++) {
        std::cout << bs[i];
    }
    std::cout << "" << std::endl;
    std::cout << bs.num_filled() << std::endl;
    return 0;
}

