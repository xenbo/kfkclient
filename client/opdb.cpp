//
// Created by dongbo on 19-12-5.
//
#include <iostream>
#include "opdb.h"
#include "z_hglog.h"


extern "C" {
extern void
cursor_all(void *key, int klen, void *val, int vlen);
}

int opdb::initdb(const std::string &dbname, int mmsize) {
    int rc;
    rc = mdb_env_create(&env);
    rc = mdb_env_set_maxreaders(env, 100);
    rc = mdb_env_set_mapsize(env, mmsize * 1024 * 1024);
    rc = mdb_env_set_maxdbs(env, 4);
    rc = mdb_env_open(env, dbname.c_str(), MDB_FIXEDMAP | MDB_NOSYNC, 0664);

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
    T_LOGI(keystr << "," << valuestr)

    int rc;
    MDB_val key, data;
    char keytmp[256] = {0};
    char valtmp[1024] = {0};

    memcpy(keytmp, keystr.c_str(), keystr.length() < sizeof(keytmp) - 1 ? keystr.length() : sizeof(keytmp) - 1);
    memcpy(valtmp, valuestr.c_str(),
           valuestr.length() < sizeof(valtmp) - 1 ? valuestr.length() : sizeof(valtmp) - 1);


    rc = mdb_txn_begin(env, NULL, 0, &txn);
    rc = mdb_dbi_open(txn, NULL, 0, &dbi);

    key.mv_size = strlen(keytmp);
    key.mv_data = keytmp;
    data.mv_size = strlen(valtmp);//sizeof(valtmp);
    data.mv_data = valtmp;

    if (MDB_KEYEXIST == mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE)) {
        rc = MDB_KEYEXIST;
    }


    mdb_txn_commit(txn);
    mdb_dbi_close(env, dbi);

    return rc;
}


int opdb::delkey(const std::string &keystr) {
    int rc;
    MDB_val key, data;
    char keytmp[256] = {0};

    memcpy(keytmp, keystr.c_str(), keystr.length() < sizeof(keytmp) - 1 ? keystr.length() : sizeof(keytmp) - 1);

    rc = mdb_txn_begin(env, NULL, 0, &txn);
    rc = mdb_dbi_open(txn, NULL, 0, &dbi);

    key.mv_size = strlen(keytmp);
    key.mv_data = keytmp;


    if (MDB_NOTFOUND == mdb_del(txn, dbi, &key, nullptr)) {
        T_LOGW("MDB_NOTFOUND:" << keystr)
    }

    T_LOGI(keystr)
    rc = mdb_txn_commit(txn);
    mdb_dbi_close(env, dbi);

    return rc;
}


int opdb::getkey(const std::string &keystr, std::string *valuestr) {

    int rc;
    MDB_val key, data;
    char keytmp[256] = {0};
    char valtmp[1024] = {0};

    memcpy(keytmp, keystr.c_str(), keystr.length() < sizeof(keytmp) - 1 ? keystr.length() : sizeof(keytmp) - 1);

    mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
    mdb_dbi_open(txn, NULL, 0, &dbi);

    key.mv_size = strlen(keytmp);
    key.mv_data = keytmp;
    data.mv_size = sizeof(valtmp);
    data.mv_data = valtmp;


    if (MDB_NOTFOUND == mdb_get(txn, dbi, &key, &data)) {
        T_LOGW("MDB_NOTFOUND:" << keystr)
        rc = MDB_NOTFOUND;
    } else {
        valuestr->assign((const char *) data.mv_data, data.mv_size);
        rc = 0;
    }

    T_LOGI(keystr << "," << *valuestr)

    mdb_txn_commit(txn);
    mdb_dbi_close(env, dbi);
    return rc;
}

int opdb::cursor_(std::map<std::string, std::string> &list) {

    MDB_cursor *cursor;
    mdb_txn_begin(env, NULL, MDB_RDONLY , &txn);
    mdb_dbi_open(txn, NULL, 0, &dbi);
    mdb_cursor_open(txn, dbi, &cursor);

    MDB_val key, data;
    char keytmp[256] = {0};
    char valtmp[1024] = {0};

    key.mv_size = sizeof(keytmp);
    key.mv_data = keytmp;
    data.mv_size = sizeof(valtmp);
    data.mv_data = valtmp;

    int rc = -1;
    while ((rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT)) == 0) {
        T_LOGI("key:" << std::string((char *) key.mv_data, key.mv_size) << "," << "val:"
                      << std::string((char *) data.mv_data, data.mv_size))

        list[std::string((char *) key.mv_data, key.mv_size)] = std::string((char *) data.mv_data, data.mv_size);
    }

    mdb_cursor_close(cursor);
    mdb_txn_commit(txn);
    mdb_close(env, dbi);
    return rc;
};

int opdb::get_first(std::string *keystr, std::string *valuestr) {

    MDB_cursor *cursor;
    mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
    mdb_dbi_open(txn, NULL, 0, &dbi);
    mdb_cursor_open(txn, dbi, &cursor);

    MDB_val key, data;
    char keytmp[256] = {0};
    char valtmp[1024] = {0};

    key.mv_size = sizeof(keytmp);
    key.mv_data = keytmp;
    data.mv_size = sizeof(valtmp);
    data.mv_data = valtmp;

    int rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT);
    if (rc == 0) {
        T_LOGI("key:" << std::string((char *) key.mv_data, key.mv_size) << "," << "val:"
                      << std::string((char *) data.mv_data, data.mv_size))

        keystr->assign((const char *) key.mv_data, key.mv_size);
        valuestr->assign((const char *) data.mv_data, data.mv_size);
    }

    mdb_cursor_close(cursor);
    mdb_txn_commit(txn);
    mdb_close(env, dbi);
    return rc;
};