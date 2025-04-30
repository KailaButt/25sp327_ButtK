#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "queue.h"

typedef struct node {
    void* data;
    struct node* next;
} node_t;

struct queue {
    node_t* front;
    node_t* rear;
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
    bool closed;
    int size;
};

queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var) {
    queue_t* q = malloc(sizeof(queue_t));
    if (!q) {
        perror("queue_init: malloc");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    q->mutex = mutex;
    q->cond = cond_var;
    q->closed = false;
    q->size = 0;
    return q;
}

void queue_destroy(queue_t* q) {
    while (q->front != NULL) {
        node_t* tmp = q->front;
        q->front = tmp->next;
        free(tmp->data); // Free the request inside
        free(tmp);
    }
    free(q);
}

void queue_enqueue(queue_t* q, void* req) {
    node_t* node = malloc(sizeof(node_t));
    if (!node) {
        perror("queue_enqueue: malloc");
        exit(EXIT_FAILURE);
    }
    node->data = req;
    node->next = NULL;

    LOCK_MTX(q->mutex);
    if (q->closed) {
        fprintf(stderr, "queue_enqueue called on closed queue.\n");
        free(node);
        UNLOCK_MTX(q->mutex);
        return;
    }

    if (q->rear) {
        q->rear->next = node;
    } else {
        q->front = node;
    }
    q->rear = node;
    q->size++;

    pthread_cond_signal(q->cond);
    UNLOCK_MTX(q->mutex);
}

void* queue_dequeue(queue_t* q) {
    LOCK_MTX(q->mutex);
    while (q->front == NULL && !q->closed) {
        pthread_cond_wait(q->cond, q->mutex);
    }

    if (q->front == NULL && q->closed) {
        UNLOCK_MTX(q->mutex);
        return NULL;
    }

    node_t* node = q->front;
    q->front = node->next;
    if (!q->front) {
        q->rear = NULL;
    }
    q->size--;

    UNLOCK_MTX(q->mutex);

    request_t* req = node->data;
    free(node);
    return req;
}

void queue_close(queue_t* q) {
    LOCK_MTX(q->mutex);
    q->closed = true;
    pthread_cond_broadcast(q->cond);
    UNLOCK_MTX(q->mutex);
}

bool queue_isclosed(queue_t* q) {
    LOCK_MTX(q->mutex);
    bool status = q->closed;
    UNLOCK_MTX(q->mutex);
    return status;
}

int queue_size(queue_t* q) {
    LOCK_MTX(q->mutex);
    int size = q->size;
    UNLOCK_MTX(q->mutex);
    return size;
}
