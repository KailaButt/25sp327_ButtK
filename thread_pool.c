#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "thread_pool.h"
#include "worker_thread.h"
#include "tasks.h"

struct thread_pool {
    worker_thread_t** workers;
    int num_threads;
};

thread_pool_t* thread_pool_init(int num_threads) {
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));
    if (!pool) {
        perror("thread_pool_init: malloc");
        exit(EXIT_FAILURE);
    }

    pool->workers = malloc(sizeof(worker_thread_t*) * num_threads);
    if (!pool->workers) {
        perror("thread_pool_init: workers malloc");
        exit(EXIT_FAILURE);
    }

    pool->num_threads = num_threads;

    for (int i = 0; i < num_threads; i++) {
        pool->workers[i] = create_worker_thread();
    }

    return pool;
}

void thread_pool_add(thread_pool_t* pool, void (*work_fn)(void*), void* arg) {
    request_t* req = malloc(sizeof(request_t));
    if (!req) {
        perror("thread_pool_add: malloc");
        exit(EXIT_FAILURE);
    }

    req->work_fn = work_fn;
    req->arg = arg;

    int index = rand() % pool->num_threads;
    queue_enqueue(pool->workers[index]->request_queue, req);
}

void thread_pool_destroy(thread_pool_t* pool) {
    for (int i = 0; i < pool->num_threads; i++) {
        queue_close(pool->workers[i]->request_queue);
    }

    for (int i = 0; i < pool->num_threads; i++) {
        join_worker_thread(pool->workers[i]);
    }

    free(pool->workers);
    free(pool);
}
