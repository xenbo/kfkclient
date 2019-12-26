//
// Created by dongbo on 19-12-4.
//

//
// Created by dongbo on 19-12-4.
//

extern "C" {

#include "client/for_go.h"
#include <stdio.h>
#include <unistd.h>

void
Cgo_comsumer_callback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code) {
    printf("Cgo_comsumer_callback %s %s  %lld\n", topic, msg, offset);
}

void cursor_all(void *key, int klen, void *val, int vlen) {
    printf("Cgo_comsumer_callback %s %s  \n", (char *) key, (char *) val);
}

}

#include "client/z_hglog.h"
#include <thread>


int main() {

    db_CLogThread::InitLogger("kfk");

    auto c = create_consumer("192.168.1.172:9092");
    add_consume_topic(c, "test_topicxxx", 0);


    std::thread t([&]() {
        start_consumer(c);
    });


    auto p = create_producer("192.168.1.172:9092");
    add_produce_topic(p, "test_topicxxx");

    for (int i = 0; i < 1000000; ++i) {
        send_msg(p, "xxxxxxxxxxxxxxxxxxxxxxxx", "test_topicxxx");
    }


    std::this_thread::sleep_for(std::chrono::seconds(30900));


    return 0;
}
