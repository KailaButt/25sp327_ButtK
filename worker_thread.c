#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "worker_thread.h"

static int thread_counter = 1;  // for assigning visible thread numbers
static pthread_mutex_t thread_id_lock = PTHREAD_MUTEX_INITIALIZER;

static void* worker_func(void* arg) {
    worker_thread_t* worker = (worker_thread_t*)arg;

    int local_id;
    int processed_count = 0;

    LOCK_MTX(&thread_id_lock);
    local_id = thread_counter++;
    UNLOCK_MTX(&thread_id_lock);

    printf("thread-%d starting\n", local_id);

    while (true) {
        request_t* req = (request_t*)queue_dequeue(worker->request_queue);
        if (req == NULL) {
            break;
        }

        req->work_fn(req->arg);
        free(req);
        processed_count++;
    }

    printf("thread-%d exiting. Processed %d requests.\n", local_id, processed_count);
    return NULL;
}

worker_thread_t* create_worker_thread() {
    worker_thread_t* worker = malloc(sizeof(worker_thread_t));
    if (!worker) {
        perror("create_worker_thread: malloc");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    pthread_cond_t* cond = malloc(sizeof(pthread_cond_t));

    if (!mutex || !cond) {
        perror("create_worker_thread: mutex/cond malloc");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(EXIT_FAILURE);
    }

    worker->request_queue = queue_init(mutex, cond);

    int ret = pthread_create(&worker->thread_id, NULL, worker_func, worker);
    if (ret != 0) {
        handle_error_en(ret, "pthread_create");
    }

    return worker;
}

void join_worker_thread(worker_thread_t* worker) {
    int ret = pthread_join(worker->thread_id, NULL);
    if (ret != 0) {
        handle_error_en(ret, "pthread_join");
    }

    queue_destroy(worker->request_queue);
    free(worker);
}
