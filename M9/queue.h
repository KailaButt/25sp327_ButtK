/**
 * This is the public-facing header file for the queue.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Define node_t structure (generic or specific)
typedef struct node {
    void* data; // or request_t* data if queue is specific
    struct node* next;
} node_t;

// Define queue_t structure
typedef struct queue {
    node_t* front;
    node_t* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool closed;
    int size;
} queue_t;

/// @brief Create a queue with the given the given mutex and condition variables.
extern queue_t* queue_init(pthread_mutex_t* mutex,
                           pthread_cond_t* cond_var);

/// @brief Places the data at the back of the queue.
extern void queue_enqueue(queue_t* que, void* data);

/// @brief Removes the data at the head of the queue and returns it.
extern void* queue_dequeue(queue_t* que);

/// @brief Removes the data at the head of the queue and returns it.
/// Blocks if the queue is empty until new data arrives.
extern void* queue_dequeue_wait(queue_t* que);

/// @brief Returns the number of elements currently in the queue.
extern int queue_size(queue_t* que);

/// @brief Destroys the given queue.
extern void queue_destroy(queue_t* que);

/// @brief Marks the queue as closed so that no more requests are accepted.
extern void queue_close(queue_t* que);

/// @brief Returns true if the queue is closed, false otherwise.
extern bool queue_isclosed(queue_t* que);

#endif // QUEUE_H

