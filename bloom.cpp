#include <iostream>
#include <bitset>
#include <stdio.h>
#include <openssl/md5.h>
#include "bloom_filter.h"
#include "atomic_bitset.h"
#include "scalable_bloom_filter.h"
#include "copyable_atomic.h"
#include <vector>
#include <thread>

struct base {

    virtual void print() {
        std::cout << "base" << std::endl;
    }
};

struct derv : public base {

    virtual void print() {
        std::cout << "derv" << std::endl;
    }

};



int main(void) {

    copyable_atomic<int> x(3544);
    copyable_atomic<int>y(x);
    std::cout << "y: " << y << std::endl;

    bloom_filter bf(0.01, 1000);
    bf.insert("wat");
    bf.insert("wat2");
    bloom_filter bf2(0.01, 1000);
    bf2.insert("abc");

    atomic_bitset bs1(10);
    atomic_bitset bs2(10);
    bs1.set(1, 1);
    bs1.set(2, 1);
    bs2.set(3, 1);

    std::cout << bs1.num_filled() << std::endl;
    swap(bs1, bs2);
    std::cout << bs1.num_filled() << std::endl;
    std::cout << bs2.num_filled() << std::endl;
    //std::swap(bf, bf2);

    //bf.insert("hello");
    //bf.insert("dsfoier");
    //bf.insert("2340x9xw");
    //bf.insert("adfoiww3");
    //bf.insert("azxkjjww");
    //bf.insert("hellowrold");
    //bf.insert("wateroisawtoi");
    //bf.insert("worldlwld");
    //bf.insert("aodfierddd");
    //bf.insert("aowiercdidoiioo");
    //bf.insert("222wwxoxoisjjdls");
    //bf.insert("ssiduaooowrqwe");
    //bf.insert("a");
    //bf.insert("av");
    //bf.insert("23edi");
    //bf.insert("dddfkk");
    //bf.insert("eeeq");
    //bf.insert("yyy99");
    //bf.insert("eeeqood");
    //bf.insert("sdoieee");
    //bf.insert("d993jss");
    //bf.insert("heldoa");
    //bf.insert("ssdoo100");
    //std::cout << bf.contains("wer9xcc") << std::endl;
    //std::cout << bf.fill_ratio() << std::endl;
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

