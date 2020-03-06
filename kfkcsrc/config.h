//
// Created by dongbo on 19-12-17.
//

#ifndef KFKCLIENT_CONFIG_H
#define KFKCLIENT_CONFIG_H

#define WITHOUT_OPTIMIZATION 0
#define ENABLE_DEVEL 0
#define ENABLE_REFCNT_DEBUG 0
#define ENABLE_SHAREDPTR_DEBUG 0

#define HAVE_ATOMICS_32 0
#define HAVE_ATOMICS_32_SYNC 0

#if (HAVE_ATOMICS_32)
# if (HAVE_ATOMICS_32_SYNC)
#  define ATOMIC_OP32(OP1,OP2,PTR,VAL) __sync_ ## OP1 ## _and_ ## OP2(PTR, VAL)
# else
#  define ATOMIC_OP32(OP1,OP2,PTR,VAL) __atomic_ ## OP1 ## _ ## OP2(PTR, VAL, __ATOMIC_SEQ_CST)
# endif
#endif

#define HAVE_ATOMICS_64 0
#define HAVE_ATOMICS_64_SYNC 0

#if (HAVE_ATOMICS_64)
# if (HAVE_ATOMICS_64_SYNC)
#  define ATOMIC_OP64(OP1,OP2,PTR,VAL) __sync_ ## OP1 ## _and_ ## OP2(PTR, VAL)
# else
#  define ATOMIC_OP64(OP1,OP2,PTR,VAL) __atomic_ ## OP1 ## _ ## OP2(PTR, VAL, __ATOMIC_SEQ_CST)
# endif
#endif

#define WITH_PKGCONFIG 0
#define WITH_HDRHISTOGRAM 0
#define WITH_ZLIB 0
#define WITH_ZSTD 0
#define WITH_LIBDL 0
#define WITH_PLUGINS 0
#define WITH_SNAPPY 1
#define WITH_SOCKEM 1
#define WITH_SSL 0
#define WITH_SASL 0
#define WITH_SASL_SCRAM 0
#define WITH_SASL_OAUTHBEARER 0
#define WITH_SASL_CYRUS 0
#define WITH_LZ4_EXT 0
#define HAVE_REGEX 0
#define HAVE_STRNDUP 0
#define HAVE_PTHREAD_SETNAME_GNU 0
#define HAVE_PTHREAD_SETNAME_DARWIN 0
#define WITH_C11THREADS 0
#define WITH_CRC32C_HW 0
#define SOLIB_EXT ".so"
#define BUILT_WITH  ""


#endif //KFKCLIENT_CONFIG_H
