cmake_minimum_required(VERSION 3.13)
project(write_leveldb)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DLEVELDB_PLATFORM_POSIX=1)

include_directories(leveldb_by_db)
include_directories(leveldb_by_db/include)


aux_source_directory(leveldb_by_db/db src)
aux_source_directory(leveldb_by_db/table src)
aux_source_directory(leveldb_by_db/util src)
aux_source_directory(leveldb_by_db/db src)

add_library(leveldb_by_db STATIC ${src})

add_executable(write_leveldb main.cpp)
target_link_libraries(write_leveldb leveldb_by_db  pthread)