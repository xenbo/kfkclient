//
// Created by dongbo on 19-12-4.
//
extern "C" {
#include "for_go.h"
}

#include "Producer.h"
#include "Consumer.h"
#include "opdb.h"
#include "z_hglog.h"

#include <thread>
#include <stdlib.h>
#include <cstring>
#include <cassert>


struct Producer_t : public Producer {

};


struct Consumer_t : public Consumer {

};


Producer_t *create_producer(const char *kfk_addr) {
    auto p = new Producer_t();
    p->create_producer(kfk_addr);

    return p;
}

Consumer_t *create_consumer(const char *kfk_addr) {
    auto c = new Consumer_t();
    c->create_consumer(kfk_addr);
    return c;
}

unsigned long long get_consumer_hash_code(Consumer_t *c) {
    assert(c != nullptr);
    return c->get_hashcode();
}

void add_produce_topic(Producer_t *p, const char *topic) {
    p->add_produce_topic(std::string(topic));
}

void add_consume_topic(Consumer_t *c, const char *topic, long long offset) {
    c->add_consume_topic(std::string(topic), offset);
}

void send_msg(Producer_t *p, const char *msg, const char *topic) {
    p->send_msg(std::string(msg), std::string(topic));
    p->flush();
}

void flush(Producer_t *p) {
    p->flush();
}

void start_consumer(Consumer_t *c) {
    c->start_consumer();
}


struct Storage_t : opdb {


};

Storage_t *storage_init(const char *dbname, int mmsize) {
    std::string cmd = "mkdir " + std::string(dbname);
    system(cmd.c_str());

    auto s = new Storage_t;

    s->initdb(dbname, mmsize);
    return s;
}


int storage_setkey(Storage_t *db, const char *key, int klen, const char *val, int vlen) {
    return db->setkey(std::string(key, klen), std::string(val, vlen));
}


int storage_getkey(Storage_t *db, const char *key, int klen, void *val, int vlen) {
    std::string val0;
    int l = 0;
    int rc = db->getkey(std::string(key, klen), &val0);
    T_LOGI(rc << ",key:" << std::string(key, klen))
    if (rc == 0) {
        l = val0.length() < vlen - 1 ? val0.length() : vlen - 1;
        memcpy(val, val0.c_str(), l);
        T_LOGI(rc << ",val:" << val0)
    } else {
        l = -1;
    }

    return l;
}


void init() {
    db_CLogThread::InitLogger("rdkafka", true);
}


