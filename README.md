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
- sudo make install  

Installing: /usr/share/pkgconfig/syrdkafka.pc  
Installing: /usr/local/lib/libsyrdkafka.a    
  

# Check with command  pkg-config
[dongbo@localhost]$ pkg-config --list-all |grep syrdkafka  
syrdkafka                 libsyrdkafka - The  syKafka C/C++ library


# go_kfk_client need libsyrdkafka
https://github.com/xenbo/go_kfk_client


