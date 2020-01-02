//
// Created by dongbo on 19-12-3.
//

#ifndef KFKCLIENT_PRODUCER_H
#define KFKCLIENT_PRODUCER_H

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <string.h>
#include <unistd.h>
#include <cstdio>

#include "kfkcsrc/rdkafka.h"

class Producer {

private:
    static void message_receipt_cbk(rd_kafka_t *kf_producer, const rd_kafka_message_t *kf_message, void *opaque);

    rd_kafka_t *create_kafka_producer(std::string kfk_addr = "192.168.1.172:9092");

    rd_kafka_topic_t *create_topic(const std::string &topic);

public:

    int add_produce_topic(const std::string &topic);

    int create_producer(std::string kfk_addr);

    int send_msg(const std::string &message, std::string topic, long long key=-1);

    int flush(int t = 0);

private:
    std::map<std::string, rd_kafka_topic_t *> p_kafka_topics;
    rd_kafka_t *kf_producer{nullptr};
};


#endif //KFKCLIENT_PRODUCER_H
