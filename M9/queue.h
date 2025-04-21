#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct request {
    int id;
    int type;
    int data;
} request_t;

typedef struct node {
    request_t* req;
    struct node* next;
} node_t;

typedef struct queue {
    node_t* front;
    node_t* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} queue_t;

queue_t* queue_create();
void queue_destroy(queue_t* q);
void queue_enqueue(queue_t* q, request_t* req);
request_t* queue_dequeue(queue_t* q);

#endif

