//
// Created by dongbo on 19-12-4.
//

#ifndef KFKCLIENT_FOR_GO_H
#define KFKCLIENT_FOR_GO_H

//kafka client
typedef struct Producer_t Producer_t;
typedef struct Consumer_t Consumer_t;

Producer_t *create_producer(const char *kfk_addr);

Consumer_t *create_consumer(const char *kfk_addr);

unsigned long long get_consumer_hash_code(Consumer_t *c);

void add_produce_topic(Producer_t *p, const char *topic);

void add_consume_topic(Consumer_t *c, const char *topic,long long offset);

void send_msg(Producer_t *p, const char *msg, const char *topic);

void flush(Producer_t *p);

void start_consumer(Consumer_t *c);


//storage
typedef struct Storage_t Storage_t;

Storage_t *storage_init(const char *dbname, int mmsize);

int storage_setkey(Storage_t *db, const char *key, int klen, const char *val, int vlen);

int storage_getkey(Storage_t *db, const char *key, int klen ,  void *val ,int vlen) ;

//log
void init();



#endif //KFKCLIENT_FOR_GO_H
