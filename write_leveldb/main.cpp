#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "leveldb/db.h"
#include "leveldb/cache.h"


struct LDB {
    int num;
    std::string dataBaseTopic;
    leveldb::DB *db;
    leveldb::Options options;
    leveldb::WriteOptions woptions;

    LDB(std::string Topic) {

        num = 0;
        woptions.sync = false;
        options.create_if_missing = true;
        options.write_buffer_size = 5 * 1024 * 1024;
        options.block_size = 1024 * 1024;
        options.block_cache = leveldb::NewLRUCache(10 * 1024 * 1024);

        char tmpPAth[1024];
        sprintf(tmpPAth, "%s/%s", "./", Topic.c_str());
        std::string path = tmpPAth;

        char cmd[1024];
        sprintf(cmd, "mkdir -p %s", tmpPAth);
        system(cmd);

        leveldb::Status status = leveldb::DB::Open(options, path, &db);
        if (!status.ok()) {
            std::cout << "topic:" << path << std::endl;
        }

        assert(status.ok());
    }

    int _put(std::string key, std::string &data) {
        leveldb::WriteOptions writeOp;
        writeOp.sync = false;

        leveldb::Status a = db->Put(writeOp, key, data);
        if (a.ok()) return 0;

//    DATATEST[key] =data;
        return -1;
    }

    int _get(std::string key, std::string *data) {
        leveldb::Status a = db->Get(leveldb::ReadOptions(), key, data);
        if (a.ok()) return 0;
        if (a.IsNotFound()) {
            return -2;
        }

//    *data = DATATEST[key];
        return -1;
    }

    int _del_first() {
        auto it1 = db->NewIterator(leveldb::ReadOptions());
        leveldb::WriteOptions writeOp;
        writeOp.sync = false;
        std::shared_ptr<leveldb::Iterator> it(it1);

        it->SeekToFirst();
        db->Delete(writeOp, it1->key());
        return 0;
    }

};


int main() {

    std::vector<std::string> topic_list;
    for (int i = 0; i < 60; ++i) {
        topic_list.push_back("topic_" + std::to_string(i));
    }

    std::vector<std::unique_ptr<LDB>> db_list;
    for (int i = 0; i < 60; ++i) {
        db_list.push_back(std::make_unique<LDB>(topic_list[i]));
    }

    int n = 0;

    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 60; ++j) {
            std::string key = std::to_string(i);
            std::string val = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxals;dkflasdkf"
                              "xxxxxxxxxxddddddddddddddddddddddddddddddddddddddddddddddddasldjfklasdjfkl"
                              "sadfffffffffffffffasdfasdfadsfasdfsdafasdfasdfasdfasxxxxxxxxxxxxxx_" + std::to_string(i);

            db_list[j]->_put(key, val);
            db_list[j]->_del_first();
            std::cout << n++ << ":" << i << std::endl;
        }
    }

    std::cout << time(0) << std::endl;


    return 0;
}