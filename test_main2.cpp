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

void Cgo_comsumer_callback(const char *topic, long long offset, const char *msg, int len) {
    printf("Cgo_comsumer_callback %s %s  %lld\n", topic, msg, offset);
}
}

#include "client/z_hglog.h"
#include <thread>


int main() {

    db_CLogThread::InitLogger("kfk");

    auto p = create_producer("api.yangyongbao.cn:9092");
    add_produce_topic(p, "test_topicxxx");

    for (int i = 0; i < 10000; ++i) {
        usleep(1000);
        send_msg(p, "xxxxxxxxxxxxxxxxxxxxxxxx", "test_topicxxx");
    }
    flush(p);

    auto c = create_consumer("api.yangyongbao.cn:9092");
    add_consume_topic(c, "test_topicxxx", 0);

    printf("%lld\n",get_consumer_hash_code(c));


    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::thread t([&]() {
        start_consumer(c);
    });


    std::this_thread::sleep_for(std::chrono::seconds(30900));


    return 0;
}
