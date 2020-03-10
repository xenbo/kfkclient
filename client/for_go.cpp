//
// Created by dongbo on 19-12-4.
//


#include <iostream>
#include <thread>
#include <stdlib.h>
#include <cstring>
#include <cassert>
#include <mutex>
#include <map>
#include <atomic>
#include <boost/filesystem.hpp>


#include "for_go.h"
#include "Producer.h"
#include "Consumer.h"
#include "opdb.h"
#include "z_hglog.h"


static const int num_max = 50000;
static const std::string db_dir = "topic_cache/";
static std::string kafka_addr = "";


struct Producer_t : public Producer {

};


struct Consumer_t : public Consumer {

};

static Producer_t *get_producer(const std::string &topic) {
    static std::atomic_bool f = {false};
    static std::mutex pmux;
    static Producer_t p;

    std::lock_guard<std::mutex> lock(pmux);
    if (!f) {
        f = true;
        p.create_producer(kafka_addr);
    }

    if (!topic.empty()) {
        p.add_produce_topic(topic);
    }

    return &p;
}


Producer_t *create_producer() {
    return get_producer("");
}

Consumer_t *create_consumer() {
    auto c = new Consumer_t();
    c->create_consumer(kafka_addr);
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


/////////////////////////////////////////////////////////////////////////////////
struct Storage_t : opdb {


};

static Storage_t *get_db(const char *dbname, unsigned int mmsize) {
    static std::mutex mux;
    static std::map<std::string, Storage_t *> db_list;
    static const int db_size = 200;


    std::lock_guard<std::mutex> lock(mux);

    auto it = db_list.find(dbname);
    if (it == db_list.end()) {

        auto s = new Storage_t;
        std::string path = db_dir + dbname;
        T_LOGI("apath:"<<path)
        try {
            if(!boost::filesystem::exists(db_dir))
                boost::filesystem::create_directory(db_dir);
            if(!boost::filesystem::exists(path))
                boost::filesystem::create_directory(path);
        } catch (std::exception &e) {
            T_LOGI(e.what())
        }


        if (mmsize > db_size)
            mmsize = db_size;

        s->initdb(path, mmsize);
        db_list[dbname] = s;
        return s;
    }

    return it->second;
}


Storage_t *storage_init(const char *dbname, int mmsize) {
    return get_db(dbname, mmsize);
}


int storage_setkey(Storage_t *db, const char *key, int klen, const char *val, int vlen) {
    return db->setkey(std::string(key, klen), std::string(val, vlen));
}


int storage_getkey(Storage_t *db, const char *key, int klen, void *val, int vlen) {
    std::string val0;
    int l = 0;
    int rc = db->getkey(std::string(key, klen), &val0);
    if (rc == 0) {
        l = val0.length() < vlen - 1 ? val0.length() : vlen - 1;
        memcpy(val, val0.c_str(), l);
        T_LOGI("rc:" << rc << ",key:" << std::string(key, klen) << ",val:" << (char *) val)
    } else {
        T_LOGW("rc:" << rc << ",key:" << std::string(key, klen))
        l = -1;
    }

    return l;
}


int storage_delkey(Storage_t *db, const char *key, int klen) {
    return db->delkey(std::string(key, klen));
}

int storage_cursor(Storage_t *db) {
    std::map<std::string, std::string> v;

    return db->cursor_(v);
}


int storage_first(Storage_t *db, void *key, int klen, void *val, int vlen) {
    std::string key0;
    std::string val0;

    int l = 0;
    int rc = db->get_first(&key0, &val0);
    if (rc == 0) {
        l = key0.length() < klen - 1 ? key0.length() : klen - 1;
        memcpy(key, key0.c_str(), l);

        l = val0.length() < vlen - 1 ? val0.length() : vlen - 1;
        memcpy(val, val0.c_str(), l);

        T_LOGI("rc:" << rc << ",key:" << key0 << ",val:" << val0)
    }

    return rc;
}


int send_msg_callback(const std::string &topic, long long nkey) {

    Storage_t *db = get_db(topic.c_str(), -1);
    char id[20] = {0};
    sprintf(id, "%.20lld", nkey);
    std::string key = topic + std::string(id);
    int rc = storage_delkey(db, key.c_str(), key.length());

    T_LOGI(key)

    return rc;
}


int send_msg_with_cache(Producer_t *p, const char *msg, const char *topic) {
    const std::string desc = "~~zz" + std::string(topic);
    Storage_t *db = get_db(topic, -1);

    char desc_tmp[1024] = {0};
    long long num = 0;
    int l = storage_getkey(db, desc.c_str(), desc.length(), desc_tmp, sizeof(desc_tmp));
    if (l > 0) {
        num = std::atol(desc_tmp);
    }

    {   //msg
        char id[20] = {0};
        sprintf(id, "%.20lld", num);
        std::string key = topic + std::string(id);
        storage_setkey(db, key.c_str(), key.length(), msg, strlen(msg));

        //next offset = num + 1
        sprintf(desc_tmp, "%lld", num + 1);
        storage_setkey(db, desc.c_str(), desc.length(), desc_tmp, sizeof(desc_tmp));
    }

    while (num > num_max) {
        char ktmp[256] = {0};
        char vtmp[1024] = {0};
        storage_first(db, ktmp, sizeof(ktmp), vtmp, sizeof(vtmp));
        long long offset = std::atol(&ktmp[strlen(topic)]);
        if (num - offset > num_max)
            storage_delkey(db, ktmp, strlen(ktmp));
        else
            break;
    }

    p->send_msg(std::string(msg), std::string(topic), num);
    p->flush();

    return 0;
}


static void re_send(const std::string &topic) {
    std::map<std::string, std::string> v;
    auto db = get_db(topic.c_str(), -1);
    db->cursor_(v);

    auto p = get_producer(topic);
    for (auto &node : v) {
        if (node.first.find("~~zz") < 2)
            continue;
        p->send_msg(node.second, topic, -1);
        T_LOGI("topic:" << node.first << ",msg:" << node.second)
    }
}


void recover() {

    if (!boost::filesystem::exists("topic_cache"))
        boost::filesystem::create_directory("topic_cache");

    auto demo_status = [&](const boost::filesystem::path &p, boost::filesystem::file_status s) {
//        if (boost::filesystem::is_regular_file(s)) std::cout << " is a regular file\n";
//        if (boost::filesystem::is_directory(s)) std::cout << " is a directory\n";
//        if (boost::filesystem::is_block_file(s)) std::cout << " is a block device\n";
//        if (boost::filesystem::is_character_file(s)) std::cout << " is a character device\n";
//        if (boost::filesystem::is_fifo(s)) std::cout << " is a named IPC pipe\n";
//        if (boost::filesystem::is_socket(s)) std::cout << " is a named IPC socket\n";
//        if (boost::filesystem::is_symlink(s)) std::cout << " is a symlink\n";
//        if (!boost::filesystem::exists(s)) std::cout << " does not exist\n";


    };

    for (auto &p : boost::filesystem::directory_iterator("topic_cache")) {
        boost::filesystem::file_status s = p.symlink_status();
//        demo_status(*it, it->symlink_status());
        T_LOGI(p.path())

        auto i = p.path().string().find("/");
        if (i != std::string::npos) {
            auto topic_dbname = p.path().string().substr(i + 1);
            re_send(topic_dbname);
        }
    }
}

void set_kfk_addr(const char *addr) {
    db_CLogThread::InitLogger("rdkafka", true);
    kafka_addr = addr;
}


