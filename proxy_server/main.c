#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "common.h"
#include "http_proxy.h"
#include "queue.h"
#include "thread_pool.h"

#define LISTEN_PORT 8080

static int server_socket;
static bool shutdown_requested = false;

void sigint_handler(int signum) {
    (void)signum;
    shutdown_requested = true;
    close(server_socket);
    printf("\nSIGINT received. Server shutting down gracefully...\n");
}

int main(int argc, char* argv[]) {
    int num_threads = 2;
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

    queue_t* request_queue = queue_init(&mutex, &cond_var);
    thread_pool_t* pool = thread_pool_init(num_threads);

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    struct sockaddr_in server_addr;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(LISTEN_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 100) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Proxy server listening on port %d\n", LISTEN_PORT);

    while (!shutdown_requested) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int* client_socket = malloc(sizeof(int));
        if (!client_socket) continue;

        *client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (*client_socket == -1) {
            free(client_socket);
            if (shutdown_requested) break;
            perror("accept");
            continue;
        }

        request_t* request = malloc(sizeof(request_t));
        if (!request) {
            free(client_socket);
            continue;
        }

        request->handler = handle_client_request_thread;
        request->arg = client_socket;

        queue_enqueue(request_queue, request);
    }

    thread_pool_destroy(pool);
    queue_destroy(request_queue);
    close(server_socket);

    return EXIT_SUCCESS;
}
