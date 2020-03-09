# kfkclient （Libsyrdkafka）

# Introduction
libsyrdkafka comes from librdkafka.  https://github.com/edenhill/librdkafka
When producing a message, write the data to the cache first, when Kafka confirms. libsyrdkafka clears the cache


# Only build libsyrdkafka from source code
Build from source Requirements
 The GNU toolchain
 
 GNU make
 
 pthreads
 
 C ++ compiler needs to support C ++ 17, like g ++ 7


# for example: 
- cd kfkclient 
- mkdir build 
- cd build 
- cmake .. 
- make -j 4


# go_kfk_client need libsyrdkafka
sudo cp ./libsyrdkafka.a  $GOPATH/pkg/mod/github.com/xenbo/go_kfk_client@v0.0.0-20200306042118-846a1810099c/lib
