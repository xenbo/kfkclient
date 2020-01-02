//
// Created by dongbo on 19-12-3.
//

#ifndef KFKCLIENT_CONSUMER_H
#define KFKCLIENT_CONSUMER_H

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <atomic>
#include <mutex>


#include "kfkcsrc/rdkafka.h"

class Consumer {


private:
    static void get_eof_callback(rd_kafka_message_t *rkmessage, void *opaque);

    static void consumer_callback(rd_kafka_message_t *rkmessage, void *opaque);

    rd_kafka_topic_t *create_topic(const std::string &topic);

    rd_kafka_t *create_kafka_consumer(std::string kfk_addr);

public:

    int add_consume_topic (const std::string &topic, long long offset);

    int create_consumer(std::string kfk_addr = "192.168.1.172");

    void start_consumer();

    void get_eof();


    unsigned long long get_hashcode(){
        return (unsigned long long)(this);
    }

private:

    bool isRunning {false} ;
    rd_kafka_t *kf_consumer {nullptr};

    std::mutex mx;
    std::map<std::string, std::pair<rd_kafka_topic_t *, long long>> c_kafka_topics;
    std::map<std::string, std::atomic<long long>>  topic_eof_offset;
};


#endif //KFKCLIENT_CONSUMER_H
