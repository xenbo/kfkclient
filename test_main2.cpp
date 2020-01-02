//
// Created by dongbo on 19-12-4.
//

//
// Created by dongbo on 19-12-4.
//

#include "client/for_go.h"
#include <stdio.h>
#include <unistd.h>
#include "client/z_hglog.h"
#include <thread>

extern "C" {

void
CgoConsumerCallback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code) {
    printf("CgoConsumerCallback %s %s  %lld\n", topic, msg, offset);
}

void cursor_all(void *key, int klen, void *val, int vlen) {
    printf("CgoConsumerCallback %s %s  \n", (char *) key, (char *) val);
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


    auto p = create_producer();
    add_produce_topic(p, "test_topicxxx");

    for (int i = 0; i < 1000000; ++i) {
        send_msg(p, "xxxxxxxxxxxxxxxxxxxxxxxx", "test_topicxxx");
    }

    std::this_thread::sleep_for(std::chrono::seconds(30900));

    return 0;
}
