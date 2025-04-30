#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "thread_pool.h"
#include "tasks.h"

#define NUM_THREADS 4
#define NUM_TASKS 10

int main() {
    srand(time(NULL));

    thread_pool_t* pool = thread_pool_init(NUM_THREADS);

    for (int i = 0; i < NUM_TASKS; i++) {
        int* val = malloc(sizeof(int));
        if (!val) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        *val = i;
        thread_pool_add(pool, example_task, val);
    }

    // Give threads a little time to process tasks
    // (This could be more elegant with real shutdown signaling)
    sleep(2);

    thread_pool_destroy(pool);

    return 0;
}
