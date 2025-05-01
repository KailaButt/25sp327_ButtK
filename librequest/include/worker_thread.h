#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <pthread.h>

#include "queue.h"

typedef struct worker_thread {
    pthread_t thread_id;
    queue_t* request_queue;
} worker_thread_t;

/// @brief Creates a new worker thread.
worker_thread_t* create_worker_thread();

/// @brief Joins and frees a worker thread.
void join_worker_thread(worker_thread_t* worker);

#endif // WORKER_THREAD_H
