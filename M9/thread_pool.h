#ifndef WORKER_THREAD_POOL_H
#define WORKER_THREAD_POOL_H

#include <pthread.h>
#include <stdio.h>

#include "queue.h"
#include "worker_thread.h"

#define MAX_WORKER_THREADS 20

/*
 *  Worker thread node stored in worker_thread_pool's
 *  linked list of worker threads
 */
struct worker_thread {
    int thread_id;     // thread id - a sequentially assigned number
    pthread_t thread;  // thread identifier returned by pthread_create
};

struct thread_pool {
    int next_thread_id;                                 // next thread id to assign
    int num_threads;                                    // number of threads in pool
    struct worker_thread* threads[MAX_WORKER_THREADS];  // threads
    queue_t* que;                                       // queue used by threads
};

/*
 * Creates a worker thread pool with the given mutex,
 * condition variable and request queue.
 */
extern struct thread_pool* thread_pool_init(queue_t* que, int num_threads);

/* Performs resource clean up and cancel all worker threads. */
extern void thread_pool_destroy(struct thread_pool* pool);

#endif