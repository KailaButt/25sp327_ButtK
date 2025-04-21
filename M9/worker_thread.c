#include "worker_thread.h"

#include <string.h>
#include "common.h"

void* do_work(void* worker_thread_params) {
    struct worker_thread_params* params = (struct worker_thread_params*)worker_thread_params;
    fprintf(stderr, "thread-%d starting\n", params->thread_id);

    int req_count = 0;
    request_t* req = NULL;

    do {
        req = queue_dequeue(params->que);
        if (req != NULL) {
            req->work_fn(req->arg);
            req_count++;
            free(req);
        }
    } while (req != NULL);

    fprintf(stderr, "thread-%d exiting. Processed %d requests.\n", params->thread_id, req_count);
    pthread_exit((void*)0);
}
