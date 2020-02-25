//
// Created by dongbo on 2020-02-24.
//

#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "opdb.h"
#include "z_hglog.h"


extern "C" {
extern void
cursor_all(void *key, int klen, void *val, int vlen);
}



int opdb::initdb(const std::string &dbname, int mmsize) {
    int rc;

    evn2 = new LDB(dbname);
    dbstring = dbname;

    return rc;
}

int opdb::setkey(const std::string &keystr, const std::string &valuestr) {

    int rc = setkey0(keystr, valuestr);
    if (rc == MDB_KEYEXIST) {
        delkey(keystr);
        rc = setkey0(keystr, valuestr);
    }

    return rc;
}


int opdb::setkey0(const std::string &keystr, const std::string &valuestr) {

  int rc = evn2->_put(keystr,valuestr);

    return rc;
}


int opdb::delkey(const std::string &keystr) {

    int rc = evn2->_del(keystr);

    return rc;
}


int opdb::getkey(const std::string &keystr, std::string *valuestr) {

    int rc = evn2->_get(keystr, valuestr);

    return rc;
}

int opdb::cursor_(std::map<std::string, std::string> &list) {
    evn2->_cursor(list);

    int rc = list.size();
    return rc;
};

int opdb::get_first(std::string *keystr, std::string *valuestr) {

    int rc = evn2->_get_first(*keystr, *valuestr);

    return rc;
};