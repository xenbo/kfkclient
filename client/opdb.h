//
// Created by dongbo on 19-12-5.
//

#ifndef KFKCLIENT_OPDB_H
#define KFKCLIENT_OPDB_H

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <map>

#include "lmdb.h"

struct opdb {
    int initdb(const std::string &dbname, int mmsize) ;

    int setkey(const std::string &keystr, const std::string &valuestr) ;

    int delkey(const std::string &keystr) ;

    int getkey(const std::string &keystr, std::string *valuestr) ;

    int cursor_(std::map<std::string ,std::string> &list);

    int get_first(std::string *keystr, std::string *valuestr);

private:
    int setkey0(const std::string &keystr, const std::string &valuestr) ;

private:
    MDB_env *env;
    MDB_dbi dbi;
    MDB_txn *txn;
};


#endif //KFKCLIENT_OPDB_H
