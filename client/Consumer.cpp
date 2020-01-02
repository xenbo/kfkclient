//
// Created by dongbo on 19-12-3.
//

#include "Consumer.h"
#include "z_hglog.h"
#include <cassert>
#include <random>

extern "C" {
extern void
CgoConsumerCallback(const char *topic, long long offset, const char *msg, int len, unsigned long long consumer_code);
}


void Consumer::get_eof_callback(rd_kafka_message_t *rkmessage, void *opaque) {
    auto c = reinterpret_cast<Consumer *>(opaque);
    if (rkmessage == nullptr) {
        return;
    }

    if (rkmessage->err != 0) {
        T_LOGE("cons) Received err message:" << rkmessage->err)
        if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
            T_LOGW("now eof ")
        }
    }

    std::string topicName = rd_kafka_topic_name(rkmessage->rkt);
    std::string message((char *) rkmessage->payload, rkmessage->len);
    auto offset = rkmessage->offset;

    T_LOGI("cons) Received message for topic: " << topicName << "," << offset << ",Message: " << message)

    if (c->topic_eof_offset[topicName] < 0) {
        c->topic_eof_offset[topicName] = offset;
        T_LOGI("topic:" << topicName << ",eof offset:" << offset)
    }
}

void Consumer::consumer_callback(rd_kafka_message_t *rkmessage, void *opaque) {
//    std::cout << "cons) Received a message" << std::endl;

    auto c = reinterpret_cast<Consumer *>(opaque);
    if (rkmessage == nullptr) {
        return;
    }

    if (rkmessage->err != 0) {
        T_LOGE("cons) Received err message:" << rkmessage->err)
        if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
            T_LOGW("now eof ")
        }
    }

    std::string topicName = rd_kafka_topic_name(rkmessage->rkt);
    std::string message((char *) rkmessage->payload, rkmessage->len);

    char *key = (char *) rkmessage->key;
    auto offset = rkmessage->offset;
//    std::cout << "cons) Received message for topic: " << topicName << "," << offset << std::endl;
//    std::cout << "cons) Key: " << ((key != nullptr ? key : "no key was provided")) << std::endl;
//    std::cout << "cons) Message: " << message << std::endl;

    T_LOGI("cons) Received message for topic: " << topicName << "," << offset << ",Message: " << message)


    CgoConsumerCallback(topicName.c_str(), offset, message.c_str(), message.length(), c->get_hashcode());

}

rd_kafka_topic_t *Consumer::create_topic(const std::string &topic) {
    assert(kf_consumer != nullptr);

    // All topic configuration can be found at:
    // https://kafka.apache.org/documentation/#topic-config
    rd_kafka_topic_conf_t *kf_topic_conf = rd_kafka_topic_conf_new();
    return rd_kafka_topic_new(kf_consumer, topic.c_str(), kf_topic_conf);
}


int Consumer::add_consume_topic(const std::string &topic, long long offset) {
    assert(kf_consumer != nullptr);
    std::lock_guard<std::mutex> lock(mx);

    auto it = c_kafka_topics.find(topic);
    if (it != c_kafka_topics.end()) {
        return -1;
    }

    auto kt = create_topic(topic);
    c_kafka_topics[topic] = std::make_pair(kt, offset);
    topic_eof_offset[topic] = -1;

    return 0;
}

rd_kafka_t *Consumer::create_kafka_consumer(std::string kfk_addr) {
    // The configuration structure
    rd_kafka_conf_t *kf_conf;
    char errstr[512] = {0};

    rd_kafka_conf_res_t kf_conf_result;
    kf_conf = rd_kafka_conf_new();

    // And we need to configure the producer
    // All producer configuration can be found in:
    // https://kafka.apache.org/documentation/#producerconfigs


    kf_conf_result = rd_kafka_conf_set(kf_conf, "compression.codec", "snappy", errstr, sizeof(errstr));
    T_LOGW(errstr)
    kf_conf_result = rd_kafka_conf_set(kf_conf, "bootstrap.servers", kfk_addr.c_str(), errstr, sizeof(errstr));
    T_LOGW(errstr)
    kf_conf_result = rd_kafka_conf_set(kf_conf, "batch.num.messages", "50", errstr, sizeof(errstr));
    T_LOGW(errstr)
    kf_conf_result = rd_kafka_conf_set(kf_conf, "enable.auto.commit", "true", errstr, sizeof(errstr));
    T_LOGW(errstr)
    kf_conf_result = rd_kafka_conf_set(kf_conf, "auto.commit.interval.ms", "5000", errstr, sizeof(errstr));
    T_LOGW(errstr)

    // Need to set this extra group.id parameter - rdkafka will return error
    // when subscribing to topics if this parameter is not set.

//    std::default_random_engine e;
//    std::string name = "vvbigb-group-" + std::to_string(time(0)) + std::to_string(e());
//    kf_conf_result = rd_kafka_conf_set(kf_conf, "group.id", name.c_str(), errstr, sizeof(errstr));
//    T_LOGI(errstr)
    // No need to set message delivery callbacks

    return rd_kafka_new(RD_KAFKA_CONSUMER, kf_conf, errstr, 512);
}


int Consumer::create_consumer(std::string kfk_addr) {
    if (kf_consumer == nullptr) {
        kf_consumer = create_kafka_consumer(kfk_addr);
    }

    assert(kf_consumer != nullptr);
    return 0;
}

void Consumer::start_consumer() {

    {
        std::thread thead_get_eof([this]() {
            get_eof();
        });
        thead_get_eof.join();
    }

    isRunning = true;
    assert(kf_consumer != nullptr);
    std::lock_guard<std::mutex> lock(mx);
    T_LOGI("Starting consumer");

    for (auto &i :c_kafka_topics) {   /* Start consuming */
        auto rkt = i.second.first;
        long long offset = i.second.second;
        auto &topic = i.first;

        T_LOGI("consume topic:" << topic << ",start offset:" << offset)
        if (offset > topic_eof_offset[topic]) {
            offset = RD_KAFKA_OFFSET_TAIL_BASE;
            T_LOGW("consume topic too largest:" << topic << ",start offset:" << offset)
        }

        if (rd_kafka_consume_start(rkt, 0, offset) == -1) {
            rd_kafka_resp_err_t err = rd_kafka_last_error();
            T_LOGW(" Failed to start consuming:" << rd_kafka_err2str(err))
            if (err == RD_KAFKA_RESP_ERR__INVALID_ARG) T_LOGW(
                    "Broker based offset storage requires a group.id, add: -X group.id=yourGroup")
            exit(1);
        }
    }


    while (isRunning) {
        rd_kafka_message_t *rkmessage;
        rd_kafka_resp_err_t err;

        /* Poll for errors, etc. */
        rd_kafka_poll(kf_consumer, 0);

        for (auto &i :c_kafka_topics) {
            auto rkt = i.second.first;
            rkmessage = rd_kafka_consume(rkt, 0, 1);
            if (rkmessage) {
                consumer_callback(rkmessage, this);
                rd_kafka_message_destroy(rkmessage);
            }
        }
    }
}


void Consumer::get_eof() {

    bool isRun_get_eof = true;
    assert(kf_consumer != nullptr);
    std::lock_guard<std::mutex> lock(mx);
    T_LOGI("Starting consumer");

    for (auto &i :c_kafka_topics) {
        /* Start consuming */
        auto rkt = i.second.first;
        long long offset = i.second.second;

        if (rd_kafka_consume_start(rkt, 0, RD_KAFKA_OFFSET_TAIL_BASE - 1) == -1) {
            rd_kafka_resp_err_t err = rd_kafka_last_error();
            T_LOGW(" Failed to start consuming:" << rd_kafka_err2str(err))
            if (err == RD_KAFKA_RESP_ERR__INVALID_ARG) T_LOGW(
                    "Broker based offset storage requires a group.id, add: -X group.id=yourGroup")
            exit(1);
        }
    }


    while (isRun_get_eof) {
        rd_kafka_message_t *rkmessage;
        rd_kafka_resp_err_t err;

        /* Poll for errors, etc. */
        rd_kafka_poll(kf_consumer, 0);

        for (auto &i :c_kafka_topics) {
            auto rkt = i.second.first;
            rkmessage = rd_kafka_consume(rkt, 0, 1);
            if (rkmessage) {
                get_eof_callback(rkmessage, this);
                rd_kafka_message_destroy(rkmessage);
            }
        }


        //check all topic
        isRun_get_eof = false;
        for (auto &t:topic_eof_offset) {
            if (t.second < 0) {
                isRun_get_eof = true;
            }
        }
    }

}