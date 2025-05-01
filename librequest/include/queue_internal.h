#ifndef QUEUE_INTERNAL_H
#define QUEUE_INTERNAL_H

#include "queue.h"

typedef struct queue_node {
    void* data;
    struct queue_node* next;
} queue_node_t;

/* structure for a request queue */
struct queue {
    queue_node_t* header;      // queue header node (does not hold data)
    queue_node_t* tail;        // points to request in the queue
    int size;                  // number of requests in the queue
    bool isclosed;             // true when queue no more requests will be generated
    pthread_mutex_t* mutex;    // queue's mutex
    pthread_cond_t* cond_var;  // queue's condition variable
};

#endif
