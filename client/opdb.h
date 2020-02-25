//
// Created by dongbo on 19-12-5.
//

#ifndef KFKCLIENT_OPDB_H
#define KFKCLIENT_OPDB_H

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <map>

#include "leveldb/db.h"
#include "leveldb/cache.h"

#include "lmdb.h"


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

    int _put(const std::string &key, const std::string &data) {
        leveldb::WriteOptions writeOp;
        writeOp.sync = false;

        leveldb::Status a = db->Put(writeOp, key, data);
        if (a.ok()) return 0;

//    DATATEST[key] =data;
        return -1;
    }

    int _del(std::string key) {
        leveldb::WriteOptions writeOp;
        writeOp.sync = false;

        leveldb::Status a = db->Delete(writeOp, key);
        if (a.ok()) return 0;

//    DATATEST[key] =data;
        return -1;
    }

    int _get(const std::string &key, std::string *data) {
        leveldb::Status a = db->Get(leveldb::ReadOptions(), key, data);
        if (a.ok()) return 0;
        if (a.IsNotFound()) {
            return -2;
        }

//    *data = DATATEST[key];
        return -1;
    }


    int _get_first(std::string &key, std::string &val) {
        auto it1 = db->NewIterator(leveldb::ReadOptions());
        if (it1) {
            it1->SeekToFirst();

            key = it1->key().ToString();
            val = it1->value().ToString();
            delete it1;
            return 0;
        }


        return -1;
    }

    void _cursor(std::map<std::string, std::string> &list) {
        auto it1 = db->NewIterator(leveldb::ReadOptions());
        for (; it1->Valid(); it1->Next()) {
            list[it1->key().ToString()] = it1->value().ToString();
        }
    }

};

struct opdb {
    int initdb(const std::string &dbname, int mmsize);

    int setkey(const std::string &keystr, const std::string &valuestr);

    int delkey(const std::string &keystr);

    int getkey(const std::string &keystr, std::string *valuestr);

    int cursor_(std::map<std::string, std::string> &list);

    int get_first(std::string *keystr, std::string *valuestr);

    ~opdb() {
        mdb_env_close(env);
    }

private:
    int setkey0(const std::string &keystr, const std::string &valuestr);

private:
    MDB_env *env;
    LDB *evn2;
    std::string dbstring;
};


#endif //KFKCLIENT_OPDB_H
