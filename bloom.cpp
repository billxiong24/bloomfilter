#include <iostream>
#include <bitset>
#include <stdio.h>
#include <openssl/md5.h>
#include "bloom_filter.h"
#include "atomic_bitset.h"
#include <vector>
#include <thread>



int main(void) {
    bloom_filter bf(65536);
    bf.insert("hello");
    bf.insert("dsfoier");
    bf.insert("2340x9xw");
    bf.insert("adfoiww3");
    std::cout << bf.fill_ratio() << std::endl;
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

    //bloom_filter<65536> bf(4);
    //std::cout << (3 & 7) << std::endl;
    //bf.insert("hello234j");
    ////bf.insert("worldadfadf98");
    ////bf.insert("bbsss332");
    //std::cout << bf.contains("hello") << std::endl;
    //std::cout << bf.contains("world") << std::endl;
    //std::cout << bf.contains("hello234j") << std::endl;
    //std::cout << bf.size() << std::endl;
    return 0;
}

