#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

queue_t* queue_create() {
    queue_t* q = malloc(sizeof(queue_t));
    if (!q) {
        perror("queue_create: malloc");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
    return q;
}

void queue_destroy(queue_t* q) {
    while (q->front) {
        node_t* tmp = q->front;
        q->front = q->front->next;
        free(tmp->req);
        free(tmp);
    }
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cond);
    free(q);
}

void queue_enqueue(queue_t* q, request_t* req) {
    node_t* node = malloc(sizeof(node_t));
    if (!node) {
        perror("queue_enqueue: malloc");
        exit(EXIT_FAILURE);
    }
    node->req = req;
    node->next = NULL;

    pthread_mutex_lock(&q->mutex);
    if (q->rear) {
        q->rear->next = node;
    } else {
        q->front = node;
    }
    q->rear = node;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

request_t* queue_dequeue(queue_t* q) {
    pthread_mutex_lock(&q->mutex);
    while (!q->front) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    node_t* node = q->front;
    q->front = node->next;
    if (!q->front) {
        q->rear = NULL;
    }
    pthread_mutex_unlock(&q->mutex);

    request_t* req = node->req;
    free(node);
    return req;
}

