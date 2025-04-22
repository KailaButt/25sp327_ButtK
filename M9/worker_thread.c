#include "worker_thread.h"

// worker_thread.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "worker_thread.h"

// Function to create a worker thread
pthread_t create_worker_thread(struct worker_thread_params* params) {
    pthread_t thread;
    int result = pthread_create(&thread, NULL, do_work, (void*)params);
    if (result != 0) {
        handle_error("pthread_create failed");
    }
    return thread;
}

// Function that defines the work each thread will do
void* do_work(void* worker_thread_params) {
    struct worker_thread_params* params = (struct worker_thread_params*)worker_thread_params;
    fprintf(stderr, "thread-%d starting\n", params->thread_id);

    int req_count = 0;
    request_t* req = NULL;

    // Loop to process all requests in the queue
    do {
        req = queue_dequeue(params->que);  // Dequeue a request
        if (req != NULL) {
            req->work_fn(req->arg);  // Process the request
            req_count++;  // Increment the processed request count
            free(req);  // Free the request after processing
        }
    } while (req != NULL);  // Keep processing as long as there are requests

    fprintf(stderr, "thread-%d exiting. Processed %d requests.\n", params->thread_id, req_count);
    pthread_exit((void*)0);  // Exit the thread
}
