#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <pthread.h>
#include <stdio.h>

#include "queue.h"

struct worker_thread_params {
    int thread_id;
    queue_t* que;
};

extern void* do_work(void* thread_params);

#endif
