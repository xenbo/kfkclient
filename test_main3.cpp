//
// Created by dongbo on 19-12-5.
//


#include <iostream>
#include <thread>
#include "client/z_hglog.h"

extern "C" {
#include "client/for_go.h"

void
Cgo_comsumer_callback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code) {
    printf("Cgo_comsumer_callback %s %s  %lld\n", topic, msg, offset);
}

void cursor_all(void *key, int klen, void *val, int vlen) {
    printf("cursor_all %d   %d  %s -  %s  \n", klen, vlen, (char *) key, (char *) val);
}

}


int main() {

    db_CLogThread::InitLogger("test");

    auto s = storage_init("testdbxxx", 1000);

    std::string key = "key_";
    std::string val = "val_";


    std::thread t1([&]() {
        for (int i = 0; i < 1000; ++i) {
            std::string kt = key + std::to_string(i);
            std::string vt = val + std::to_string(i) + "XXXXXXXXXXXXXXXXXXXXXx";
            storage_setkey(s, kt.c_str(), kt.length(), vt.c_str(), vt.length());
        }
    });

//    std::thread t3([&]() {
//        for (int i = 0; i < 1000000; ++i) {
//            std::string kt = key + std::to_string(i);
//            std::string vt = val + std::to_string(i * 100) + "FFFFFFFFFFFFFFGGGGGGGGGGGGGGG";
//            odb.setkey(kt, vt);
//        }
//    });

    std::thread t2([&]() {
        for (int i = 0; i < 1000; ++i) {
            std::string kt = key + std::to_string(i);
            std::string value;

            char val[10000];
            storage_getkey(s, kt.c_str(), kt.length(), val, 10000);
            value.assign(val);

            std::cout << value << std::endl;
        }
    });


    std::this_thread::sleep_for(std::chrono::seconds(2));

    storage_cursor(s);

    std::this_thread::sleep_for(std::chrono::seconds(1000));
    return 0;
}