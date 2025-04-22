#include "thread_pool.h"
#include "common.h"

// This function initializes a thread pool by setting up the queue, the number of threads,
// and the worker threads that will process requests from the queue.
struct thread_pool* thread_pool_init(queue_t* que, int num_threads) {
    // Step 1: Allocate memory for the thread pool structure
    struct thread_pool* pool = malloc(sizeof(struct thread_pool));
    if (!pool) {
        perror("Failed to allocate memory for thread pool");
        return NULL;
    }

    // Store the queue and number of threads in the pool structure
    pool->queue = que;
    pool->num_threads = num_threads;

    // Initialize the worker threads in the pool
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) {
        perror("Failed to allocate memory for thread pool threads");
        free(pool);
        return NULL;
    }

    // Create the threads that will process tasks from the queue
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, (void*)pool) != 0) {
            perror("Failed to create thread");
            // Handle thread creation failure
            free(pool->threads);
            free(pool);
            return NULL;
        }
    }

    // Return the initialized thread pool structure
    return pool;
}

// clean up the thread pool, terminating the threads, and releasing any allocated resources.
void thread_pool_destroy(struct thread_pool* pool) {
    // Indicate to the threads that they should stop processing
    queue_close(pool->queue);

    //  Wait for all threads to finish by joining them
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    //  Free the memory allocated for the thread pool and its threads
    free(pool->threads);
    free(pool);
}
