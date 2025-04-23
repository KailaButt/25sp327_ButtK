/**
 * https://gcc.gnu.org/onlinedocs/cpp/Once-Only-Headers.html
 * If a header file happens to be included twice, the compiler
 * will process its contents twice. This is very likely to cause an error,
 * e.g. when the compiler sees the same structure definition twice.
 * Even if it does not, it will certainly waste time.
 */

 #define _GNU_SOURCE

 #ifndef COMMON_H
 #define COMMON_H
 
 // Note you may need an blank file at the end of this file
 // to eliminate "warning: backslash-newline at end of file"
 
 #include <errno.h>
 #include <stdio.h>
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
 
 #define LOCK_MTX(mtx)                                   \
     do {                                                \
         int ret = 0;                                    \
         if ((ret = pthread_mutex_lock(mtx)))            \
             handle_error_en(ret, "pthread_mutex_lock"); \
     } while (0)
 
 #define UNLOCK_MTX(mtx) \
     do {                \
         int ret = 0;                                    \
         if ((ret = pthread_mutex_unlock(mtx)))            \
             handle_error_en(ret, "pthread_mutex_unlock"); \
     } while (0)
 
 
 typedef struct request {
     void (*work_fn)(void*);
     void* arg;
 } request_t;
 
 #endif


