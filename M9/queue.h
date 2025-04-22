/**
 * This is the public facing header file.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct queue queue_t;

/// @brief Create a queue with the given the given mutex and condition variables.
/// @param mutex 
/// @param cond_var 
/// @return 
extern queue_t* queue_init(pthread_mutex_t* mutex,
                           pthread_cond_t* cond_var);

/// @brief Places the data at the back of the queue.
/// @param que the queue
/// @param data the data 
extern void queue_enqueue(queue_t* que, void* data);

/// @brief Removes the data at the head of the queue and returns it.
/// @param que the queue 
/// @return the data 
extern void* queue_dequeue(queue_t* que);

/// @brief Removes the data at the head of the queue and returns it.
/// If the queue is empty the call blocks until an element
/// enters the queue.
/// @param que the queue 
/// @return the data 
extern void* queue_dequeue_wait(queue_t* que);

/// @brief Returns the number of elements currently in the queue.
/// @param que the queue
/// @return the number elements in the queue 
extern int queue_size(queue_t* que);

/// @brief Destroys the given queue.
/// @param que the queue
extern void queue_destroy(queue_t* que);

/// @brief Marks the queue as closed so that no more requests are accepted.
/// @param que the queue 
extern void queue_close(queue_t* que);

/// @brief Returns true if the queue is closed, false otherwise.
/// @param que the queue 
/// @return true if the queue is closed 
extern bool queue_isclosed(queue_t* que);

#endif

