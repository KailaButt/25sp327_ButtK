#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "worker_thread.h"

typedef struct thread_pool thread_pool_t;

/// @brief Initialize the thread pool with a given number of worker threads.
thread_pool_t* thread_pool_init(int num_threads);

/// @brief Add a request to the thread pool.
void thread_pool_add(thread_pool_t* pool, void (*work_fn)(void*), void* arg);

/// @brief Shutdown the thread pool and clean up all resources.
void thread_pool_destroy(thread_pool_t* pool);

#endif // THREAD_POOL_H
