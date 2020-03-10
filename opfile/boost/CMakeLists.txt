cmake_minimum_required(VERSION 3.10)
project(test)
set(CMAKE_CXX_STANDARD 17)

include_directories(../..)
AUX_SOURCE_DIRECTORY(filesystem/src fsrc)

add_executable(test  ${fsrc} filesystem/example/file_status.cpp)
