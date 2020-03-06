//
// Created by dongbo on 19-12-4.
//

#include "client/Producer.h"
#include "client/Consumer.h"

#include <thread>

extern "C" {

void
CgoConsumerCallback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code) {
    printf("CgoConsumerCallback %s %s  %lld\n", topic, msg, offset);
}
}

int main() {

    Producer p;
    Consumer c;

    p.create_producer("192.168.1.172:9092");
    p.add_produce_topic("test_topic");

    for (int i = 0; i < 1000000; ++i) {
        p.send_msg("xxxxxxxxxxxxxxxxxxxxxxxx","test_topic");
    }
    p.flush();

    c.create_consumer("192.168.1.172:9092");
    c.add_consume_topic("test_topic", 0);

    std::thread t([&](){
        c.start_consumer();
    });


    std::this_thread::sleep_for(std::chrono::seconds(309));


    return 0;
}
