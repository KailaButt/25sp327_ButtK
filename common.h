#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define handle_error_en(en, msg) \
    do { \
        fprintf(stderr, "%s: %s\n", msg, strerror(en)); \
        exit(EXIT_FAILURE); \
    } while (0)

#define handle_error(msg) \
    do { \
        int en = errno; \
        fprintf(stderr, "%s: %s\n", msg, strerror(en)); \
        exit(EXIT_FAILURE); \
    } while (0)

#define LOCK_MTX(mtx) \
    do { \
        int ret = pthread_mutex_lock(mtx); \
        if (ret) handle_error_en(ret, "pthread_mutex_lock"); \
    } while (0)

#define UNLOCK_MTX(mtx) \
    do { \
        int ret = pthread_mutex_unlock(mtx); \
        if (ret) handle_error_en(ret, "pthread_mutex_unlock"); \
    } while (0)

typedef struct request {
    void (*work_fn)(void*);
    void* arg;
} request_t;

#endif 
