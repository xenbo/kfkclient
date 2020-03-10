//
// Created by dongbo on 19-12-3.
//

#include "Producer.h"
#include "z_hglog.h"
#include <cassert>


extern int send_msg_callback(const std::string &topic, long long nkey);

void Producer::message_receipt_cbk(rd_kafka_t *kf_producer, const rd_kafka_message_t *kf_message, void *opaque) {
    if (kf_message->err) {
        T_LOGW("Message delivery failed: %s" << rd_kafka_err2str(kf_message->err))
    } else {
        const std::string &keys = std::string((const char *) kf_message->key, kf_message->key_len);
        long long nkey = std::atol(keys.c_str());
        send_msg_callback(rd_kafka_topic_name(kf_message->rkt), nkey);

        T_LOGI("Message delivered,len:" << kf_message->len
                                        << ", offset: " << kf_message->offset
                                        << ",msg:" << (char *) kf_message->payload
                                        << ",nkey:" << nkey)
    }

    std::cout << "message_receipt_cbk" << std::endl;
}

rd_kafka_topic_t *Producer::create_topic(const std::string &topic) {
    assert(kf_producer != nullptr);

    // All topic configuration can be found at:
    // https://kafka.apache.org/documentation/#topic-config
    rd_kafka_topic_conf_t *kf_topic_conf = rd_kafka_topic_conf_new();
    return rd_kafka_topic_new(kf_producer, topic.c_str(), kf_topic_conf);
}

int Producer::add_produce_topic(const std::string &topic) {
    std::lock_guard<std::mutex> lock(mx);
    assert(kf_producer != nullptr);

    auto it = p_kafka_topics.find(topic);
    if (it != p_kafka_topics.end()) {
        return -1;
    }

    T_LOGI(topic)
    p_kafka_topics[topic] = create_topic(topic);

    return 0;
}

int Producer::create_producer(std::string kfk_addr) {
    std::lock_guard<std::mutex> lock(mx);
    if (kf_producer == nullptr) {
        kf_producer = create_kafka_producer(kfk_addr);
    }

    T_LOGI(kfk_addr)
    assert(kf_producer != nullptr);
    return 0;
}

rd_kafka_t *Producer::create_kafka_producer(std::string kfk_addr) { // "192.168.1.172:9092"

    // The configuration structure
    rd_kafka_conf_t *kf_conf;
    char errstr[512];

    rd_kafka_conf_res_t kf_conf_result;

    // First thing is to create a producer.
    kf_conf = rd_kafka_conf_new();

    // And we need to configure the producer
    // All producer configuration can be found in:
    // https://kafka.apache.org/documentation/#producerconfigs
    kf_conf_result = rd_kafka_conf_set(kf_conf, "compression.codec", "snappy", errstr, sizeof(errstr));
    kf_conf_result = rd_kafka_conf_set(kf_conf, "bootstrap.servers", kfk_addr.c_str(), errstr, sizeof(errstr));
    kf_conf_result = rd_kafka_conf_set(kf_conf, "batch.num.messages", "100", errstr, sizeof(errstr));

    // Set the callback to inform the result of every message sent by this
    // producer.
    rd_kafka_conf_set_dr_msg_cb(kf_conf, message_receipt_cbk);

    return rd_kafka_new(RD_KAFKA_PRODUCER, kf_conf, errstr, 512);
}


int Producer::send_msg(const std::string &message, std::string topic, long long nkey) {
    std::lock_guard<std::mutex> lock(mx);
    auto it = p_kafka_topics.find(topic);
    if (it == p_kafka_topics.end()) {
        return -1;
    }
    rd_kafka_topic_t *kf_topic = it->second;


    if (message.length() == 0) {
        return 0;
    }

    // Partition variable - this can be fixed (as in this demo), can be
    // generated by rd_kafka_msg_partitioner_*,  or RD_KAFKA_PARTITION_UA which
    // won't assign any partition to this message.
    int32_t kf_partition = RD_KAFKA_PARTITION_UA;

    // Possible flags are:
    // - RD_KAFKA_MSG_F_BLOCK: block rd_kafka_produce call.
    // - RD_KAFKA_MSG_F_FREE: free payload content after sending it.
    // - RD_KAFKA_MSG_F_COPY: copy payload content so that caller can reuse it.
    int kf_part_msg_flags = RD_KAFKA_MSG_F_FREE;

    char *msg_payload = new char[message.length() + 1];
    memset(msg_payload, 0, message.length() + 1);
    strcpy(msg_payload, message.c_str());

    size_t msg_payload_size = message.length();

    T_LOGI(topic << "," << message << "," << nkey)

    // Let's send a message
    // No keys - I don't know how to use them right now - sorry!

    int f = -1;
    if (nkey < 0) {
        f = rd_kafka_produce(kf_topic, kf_partition, kf_part_msg_flags,
                             msg_payload, msg_payload_size, nullptr, 0, nullptr);
    } else {
        char keystr[20] = {0};
        sprintf(keystr, "%.20lld", nkey);
        f = rd_kafka_produce(kf_topic, kf_partition, kf_part_msg_flags,
                             msg_payload, msg_payload_size, keystr, sizeof(keystr), nullptr);
    }

    rd_kafka_poll(kf_producer, 1000);
    return f;
}

int Producer::flush(int t) {
    assert(kf_producer != nullptr);

    return rd_kafka_flush(kf_producer, 0 * 1000);
}