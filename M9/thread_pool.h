#ifndef WORKER_THREAD_POOL_H
#define WORKER_THREAD_POOL_H

#include <pthread.h>
#include <stdio.h>

#include "queue.h"
#include "worker_thread.h"

#define MAX_WORKER_THREADS 20

struct worker_thread {
    int thread_id;
    pthread_t thread;
};

struct thread_pool {
    int next_thread_id;
    int num_threads;
    struct worker_thread threads[MAX_WORKER_THREADS];
    queue_t* que;
};

extern struct thread_pool* thread_pool_init(queue_t* que, int num_threads);
extern void thread_pool_destroy(struct thread_pool* pool);

#endif
