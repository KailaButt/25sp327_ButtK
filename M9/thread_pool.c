#include "thread_pool.h"
#include "common.h"

struct thread_pool* thread_pool_init(queue_t* que, int num_threads) {
    struct thread_pool* pool = malloc(sizeof(struct thread_pool));
    pool->num_threads = num_threads;
    pool->que = que;
    pool->next_thread_id = 0;

    for (int i = 0; i < num_threads; i++) {
        pool->threads[i].thread_id = i;
        pthread_create(&pool->threads[i].thread, NULL, do_work, &pool->threads[i]);
    }
    return pool;
}

void thread_pool_destroy(struct thread_pool* pool) {
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i].thread, NULL);
    }
    free(pool);
}
