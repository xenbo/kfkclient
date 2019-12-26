//
// Created by dongbo on 19-12-25.
//

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
    init("192.168.1.172:9092");
    auto tp1 = std::chrono::system_clock::now();
    std::string topic = "test_topicxxx";
    auto p = create_producer("192.168.1.172:9092");
    add_produce_topic(p, topic.c_str());

    for (long long i = 0; i < 1000000; ++i) {
        send_msg_with_cache(p, "xxxxxxxxxxxxxxxxxxxxxxxxxxx", topic.c_str());
    }
    auto tp2 = std::chrono::system_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::seconds>(tp2 - tp1).count();

    std::cout << d << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1000));
    return 0;
}