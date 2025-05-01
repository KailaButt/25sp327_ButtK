#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <pthread.h>

typedef struct queue queue_t;

/// @brief Create a queue with given mutex and condition variables.
queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var);

/// @brief Places the data at the back of the queue.
void queue_enqueue(queue_t* que, void* data);

/// @brief Removes and returns the data at the head of the queue.
/// If queue is empty and not closed, blocks until data is available.
/// Returns NULL if queue is closed and empty.
void* queue_dequeue(queue_t* que);

/// @brief Returns the number of elements currently in the queue.
int queue_size(queue_t* que);

/// @brief Destroys the given queue.
void queue_destroy(queue_t* que);

/// @brief Marks the queue as closed so that no more requests are accepted.
void queue_close(queue_t* que);

/// @brief Returns true if the queue is closed, false otherwise.
bool queue_isclosed(queue_t* que);

#endif // QUEUE_H
