/**
 * https://gcc.gnu.org/onlinedocs/cpp/Once-Only-Headers.html
 * If a header file happens to be included twice, the compiler 
 * will process its contents twice. This is very likely to cause an error, 
 * e.g. when the compiler sees the same structure definition twice. 
 * Even if it does not, it will certainly waste time.
 */
#ifndef COMMON_H
#define COMMON_H
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// Note you may need an blank file at the end of this file
// to eliminate "warning: backslash-newline at end of file"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define handle_error_en(en, msg)                         \
    do {                                                 \
        fprintf(stderr, "%s: (%s) %s\n",                 \
                msg, strerrorname_np(en), strerror(en)); \
        exit(EXIT_FAILURE);                              \
    } while (0)

#define handle_error(msg)                                \
    do {                                                 \
        int en = errno;                                  \
        fprintf(stderr, "%s: (%s) %s\n",                 \
                msg, strerrorname_np(en), strerror(en)); \
        exit(EXIT_FAILURE);                              \
    } while (0)
