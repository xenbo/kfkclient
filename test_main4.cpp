//
// Created by dongbo on 19-12-25.
//

//
// Created by dongbo on 19-12-5.
//


#include <iostream>
#include <thread>
#include "client/z_hglog.h"
#include "client/for_go.h"


extern "C" {

void
CgoConsumerCallback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code) {
    printf("CgoConsumerCallback %s %s  %lld\n", topic, msg, offset);
}

void cursor_all(void *key, int klen, void *val, int vlen) {
    printf("cursor_all %d   %d  %s -  %s  \n", klen, vlen, (char *) key, (char *) val);


}

}


int main() {

    set_kfk_addr("192.168.1.172:9092");
    recover();


    auto c = create_consumer();
    add_consume_topic(c, "test_topicxxx", 0);

    std::thread t([&]() {
        start_consumer(c);
    });


    auto tp1 = std::chrono::system_clock::now();
    std::string topic = "test_topicxxx";
    auto p = create_producer();
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