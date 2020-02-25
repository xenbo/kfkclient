cmake_minimum_required(VERSION 3.10)
project(kfkclient)
set(CMAKE_CXX_STANDARD 17)

include_directories(client)
include_directories(kfkcsrc)

add_definitions(-g)
#add_definitions(-W -Wall -Wno-unused-parameter -Wbad-function-cast -Wuninitialized)
#add_definitions(-fPIC)
add_definitions(-DLEVELDB_PLATFORM_POSIX=1)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(LINKERFLAGS " -static-libstdc++ -static-libgcc -Wl,-gc-sections")
    set(LINKERFLAGS "${LINKERFLAGS} -Wl,-rpath=./")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}  ${LINKERFLAGS}")
endif ()

set(
        sources
        kfkcsrc/crc32c.c
        kfkcsrc/rdaddr.c
        kfkcsrc/rdavl.c
        kfkcsrc/rdbuf.c
        kfkcsrc/rdcrc32.c
        kfkcsrc/rdkafka.c
        kfkcsrc/rdkafka_assignor.c
        kfkcsrc/rdkafka_broker.c
        kfkcsrc/rdkafka_buf.c
        kfkcsrc/rdkafka_cgrp.c
        kfkcsrc/rdkafka_conf.c
        kfkcsrc/rdkafka_event.c
        kfkcsrc/rdkafka_feature.c
        kfkcsrc/rdkafka_lz4.c
        kfkcsrc/rdkafka_metadata.c
        kfkcsrc/rdkafka_metadata_cache.c
        kfkcsrc/rdkafka_msg.c
        kfkcsrc/rdkafka_msgset_reader.c
        kfkcsrc/rdkafka_msgset_writer.c
        kfkcsrc/rdkafka_offset.c
        kfkcsrc/rdkafka_op.c
        kfkcsrc/rdkafka_partition.c
        kfkcsrc/rdkafka_pattern.c
        kfkcsrc/rdkafka_queue.c
        kfkcsrc/rdkafka_range_assignor.c
        kfkcsrc/rdkafka_request.c
        kfkcsrc/rdkafka_roundrobin_assignor.c
        kfkcsrc/rdkafka_sasl.c
        kfkcsrc/rdkafka_sasl_plain.c
        kfkcsrc/rdkafka_subscription.c
        kfkcsrc/rdkafka_timer.c
        kfkcsrc/rdkafka_topic.c
        kfkcsrc/rdkafka_transport.c
        kfkcsrc/rdkafka_interceptor.c
        kfkcsrc/rdkafka_header.c
        kfkcsrc/rdkafka_admin.c
        kfkcsrc/rdkafka_aux.c
        kfkcsrc/rdkafka_background.c
        kfkcsrc/rdkafka_idempotence.c
        kfkcsrc/rdkafka_cert.c
        kfkcsrc/rdkafka_mock.c
        kfkcsrc/rdkafka_mock_handlers.c
        kfkcsrc/rdlist.c
        kfkcsrc/rdlog.c
        kfkcsrc/rdmurmur2.c
        kfkcsrc/rdports.c
        kfkcsrc/rdrand.c
        kfkcsrc/rdregex.c
        kfkcsrc/rdstring.c
        kfkcsrc/rdunittest.c
        kfkcsrc/rdvarint.c
        kfkcsrc/snappy.c
        kfkcsrc/tinycthread.c
        kfkcsrc/tinycthread_extra.c
        kfkcsrc/xxhash.c
        client/Consumer.cpp
        client/for_go.cpp
        client/mdb.c
        client/midl.c
        client/opdb2.cpp
        client/Producer.cpp
        client/z_hglog.cpp
        kfkcsrc/config.h client/opdb2.cpp)


#set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
#configure_file("packaging/cmake/config.h.in" "${GENERATED_DIR}/config.h")


#if (WITH_SSL)
#    list(APPEND sources kfkcsrc/rdkafka_ssl.c)
#endif ()

if (WITH_HDRHISTOGRAM)
    list(APPEND sources kfkcsrc/rdhdrhistogram.c)
endif ()

if (WITH_LIBDL OR WIN32)
    list(APPEND sources kfkcsrc/rddl.c)
endif ()

if (WITH_PLUGINS)
    list(APPEND sources kfkcsrc/rdkafka_plugin.c)
endif ()

if (WIN32)
    list(APPEND sources kfkcsrc/rdkafka_sasl_win32.c)
elseif (WITH_SASL_CYRUS)
    list(APPEND sources kfkcsrc/rdkafka_sasl_cyrus.c)
endif ()

if (WITH_SASL_SCRAM)
    list(APPEND sources kfkcsrc/rdkafka_sasl_scram.c)
endif ()

if (WITH_SASL_OAUTHBEARER)
    list(APPEND sources kfkcsrc/rdkafka_sasl_oauthbearer.c)
endif ()

if (WITH_ZLIB)
    list(APPEND sources kfkcsrc/rdgz.c)
endif ()

if (WITH_ZSTD)
    list(APPEND sources kfkcsrc/rdkafka_zstd.c)
endif ()

if (NOT WITH_LZ4_EXT)
    list(APPEND sources kfkcsrc/lz4.c kfkcsrc/lz4frame.c kfkcsrc/lz4hc.c)
endif ()

if (NOT HAVE_REGEX)
    list(APPEND sources kfkcsrc/regexp.c)
endif ()

# Define flags with cmake instead of by defining them on win32_config.h
if (WITHOUT_WIN32_CONFIG)
    list(APPEND rdkafka_compile_definitions WITHOUT_WIN32_CONFIG)
    #    if (WITH_SSL)
    #        list(APPEND rdkafka_compile_definitions WITH_SSL)
    #    endif (WITH_SSL)
    if (WITH_ZLIB)
        list(APPEND rdkafka_compile_definitions WITH_ZLIB)
    endif (WITH_ZLIB)
    if (WITH_SNAPPY)
        list(APPEND rdkafka_compile_definitions WITH_SNAPPY)
    endif (WITH_SNAPPY)
    if (WITH_ZSTD)
        list(APPEND rdkafka_compile_definitions WITH_ZSTD)
    endif (WITH_ZSTD)
    if (WITH_SASL_SCRAM)
        list(APPEND rdkafka_compile_definitions WITH_SASL_SCRAM)
    endif (WITH_SASL_SCRAM)
    if (WITH_SASL_OAUTHBEARER)
        list(APPEND rdkafka_compile_definitions WITH_SASL_OAUTHBEARER)
    endif (WITH_SASL_OAUTHBEARER)
    if (ENABLE_DEVEL)
        list(APPEND rdkafka_compile_definitions ENABLE_DEVEL)
    endif (ENABLE_DEVEL)
    if (WITH_PLUGINS)
        list(APPEND rdkafka_compile_definitions WITH_PLUGINS)
    endif (WITH_PLUGINS)
endif ()


include_directories(write_leveldb/leveldb_by_db)
include_directories(write_leveldb/leveldb_by_db/include)


aux_source_directory(write_leveldb/leveldb_by_db/db sources)
aux_source_directory(write_leveldb/leveldb_by_db/table sources)
aux_source_directory(write_leveldb/leveldb_by_db/util sources)
aux_source_directory(write_leveldb/leveldb_by_db/db sources)

add_library(syrdkafka STATIC ${sources})


# Support '#include <rdkafka.h>'
target_include_directories(syrdkafka PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>")
target_compile_definitions(syrdkafka PUBLIC ${rdkafka_compile_definitions})
if (RDKAFKA_BUILD_STATIC)
    target_compile_definitions(syrdkafka PUBLIC LIBRDKAFKA_STATICLIB)
endif ()


if (WITH_HDRHISTOGRAM)
    target_link_libraries(syrdkafka PUBLIC m)
endif ()

if (WITH_ZLIB)
    find_package(ZLIB REQUIRED)
    target_link_libraries(syrdkafka PUBLIC ZLIB::ZLIB)
endif ()

if (WITH_ZSTD)
    target_link_libraries(syrdkafka PUBLIC ${ZSTD_LIBRARY})
    target_include_directories(syrdkafka PUBLIC ${ZSTD_INCLUDE_DIR})
    message(STATUS "Found ZSTD: ${ZSTD_LIBRARY}")
endif ()

#if (WITH_SSL)
#    if (WITH_BUNDLED_SSL) # option from 'h2o' parent project
#        if (NOT TARGET bundled-ssl)
#            message(FATAL_ERROR "bundled-ssl target not exist")
#        endif ()
#        target_include_directories(syrdkafka BEFORE PUBLIC ${BUNDLED_SSL_INCLUDE_DIR})
#        target_link_libraries(rdkafka PUBLIC ${BUNDLED_SSL_LIBRARIES})
#        add_dependencies(syrdkafka bundled-ssl)
#    else ()
#        find_package(OpenSSL REQUIRED)
#        target_link_libraries(syrdkafka PUBLIC OpenSSL::SSL OpenSSL::Crypto)
#    endif ()
#endif ()

if (LINK_ATOMIC)
    target_link_libraries(syrdkafka PUBLIC "-latomic")
endif ()

#find_package(Threads REQUIRED)
#target_link_libraries(syrdkafka PUBLIC Threads::Threads)

if (WITH_SASL_CYRUS)
    target_link_libraries(syrdkafka PUBLIC ${SASL_LIBRARIES})
endif ()

if (WITH_LIBDL)
    target_link_libraries(syrdkafka PUBLIC ${CMAKE_DL_LIBS})
endif ()

if (WITH_LZ4_EXT)
    target_link_libraries(syrdkafka PUBLIC LZ4::LZ4)
endif ()

# Set up path to these sources for other sub-projects (tests, examples)
# to be able to reach them.
#set(rdkafka_SRC_DIR ${PROJECT_SOURCE_DIR}
#    CACHE INTERNAL "${PROJECT_NAME} source dir" FORCE)

# Generate pkg-config file
set(PKG_CONFIG_NAME
        "librdkafka"
        )
set(PKG_CONFIG_DESCRIPTION
        "The Apache Kafka C/C++ library"
        )
set(PKG_CONFIG_VERSION
        "${PROJECT_VERSION}"
        )
set(PKG_CONFIG_REQUIRES "")
if (WITH_ZLIB)
    string(APPEND PKG_CONFIG_REQUIRES "zlib ")
endif ()
#if (WITH_SSL)
#    string(APPEND PKG_CONFIG_REQUIRES "libssl ")
#endif ()
#if (WITH_SASL_CYRUS)
#    string(APPEND PKG_CONFIG_REQUIRES "libsasl2 ")
#endif ()
#if (WITH_ZSTD)
#    string(APPEND PKG_CONFIG_REQUIRES "libzstd ")
#endif ()
#if (WITH_LZ4_EXT)
#    string(APPEND PKG_CONFIG_REQUIRES "liblz4 ")
#endif ()
set(PKG_CONFIG_CFLAGS
        "-I\${includedir}"
        )
set(PKG_CONFIG_LIBS
        "-L\${libdir} -lrdkafka"
        )
set(PKG_CONFIG_LIBS_PRIVATE
        "-lpthread"
        )
find_library(RT_LIBRARY rt)
if (RT_LIBRARY)
    string(APPEND PKG_CONFIG_LIBS_PRIVATE " -lrt")
endif ()
if (WITH_PLUGINS)
    string(APPEND PKG_CONFIG_LIBS_PRIVATE " -ldl")
endif ()
if (WITH_HDRHISTOGRAM)
    string(APPEND PKG_CONFIG_LIBS_PRIVATE " -lm")
endif ()
configure_file(
        "packaging/cmake/rdkafka.pc.in"
        "./rdkafka.pc"
        @ONLY
)
install(FILES ${GENERATED_DIR}/rdkafka.pc
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        )
#if (RDKAFKA_BUILD_STATIC)
#    set(PKG_CONFIG_NAME
#            "librdkafka-static"
#            )
#    set(PKG_CONFIG_DESCRIPTION
#            "The Apache Kafka C/C++ library (static)"
#            )
#    set(PKG_CONFIG_LIBS
#            "-L\${libdir} \${libdir}/librdkafka.a"
#            )
#    configure_file(
#            "../packaging/cmake/rdkafka.pc.in"
#            "${GENERATED_DIR}/rdkafka-static.pc"
#            @ONLY
#    )
#    install(FILES ${GENERATED_DIR}/rdkafka.pc
#            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
#            )
#endif ()


add_executable(kfkclient test_main2.cpp)
#target_link_libraries(kfkclient  syrdkafka ssl crypto dl z pthread)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(syrdkafka -fPIC -Wl,--whole-archive libstdc++.a libstdc++fs.a -Wl,--no-whole-archive)
    target_link_libraries(kfkclient syrdkafka dl z pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    target_link_libraries(kfkclient syrdkafka dl z pthread c++) ##use clang
endif ()







