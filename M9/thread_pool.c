#include "thread_pool.h"
#include "common.h"
#include <stdlib.h>
#include "worker_thread.h"
#include "queue.h"


// Function to initialize the thread pool
struct thread_pool* thread_pool_init(queue_t* que, int num_threads) {
    // Allocate memory for the thread pool
    struct thread_pool* pool = malloc(sizeof(struct thread_pool));
    if (!pool) {
        handle_error("thread_pool_init:malloc");
    }

    pool->next_thread_id = 1;  // Initialize the thread ID counter
    pool->num_threads = num_threads;  // Set the number of threads in the pool

    // Create the worker threads
    for (int i = 0; i < num_threads; i++) {
        struct worker_thread_params* params = malloc(sizeof(struct worker_thread_params));
        params->que = que;  // Set the queue for the worker thread
        params->thread_id = pool->next_thread_id;  // Set the thread ID

        // Allocate memory for each worker thread and initialize
        pool->threads[i] = malloc(sizeof(struct worker_thread));
        pool->threads[i]->thread_id = pool->next_thread_id;  // Set the worker thread ID
        pool->threads[i]->thread = create_worker_thread(params);  // Create the worker thread
        pool->next_thread_id++;  // Increment the thread ID for the next thread
    }

    return pool;  // Return the initialized thread pool
}

// Function to destroy the thread pool and clean up resources
void thread_pool_destroy(struct thread_pool* pool) {
    // Cleanup code to destroy the thread pool (if needed)
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i]->thread, NULL);  // Wait for each thread to finish
        free(pool->threads[i]);  // Free the worker thread memory
    }
    free(pool);  // Free the thread pool memory
}
