#define _GNU_SOURCE

#include "http_proxy.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"

#define BUFFER_SIZE 4096  // size of buffer for recv/send of data

static void handle_client_request(int client_socket);
static void forward_request(int target_socket, char buffer[]);
static void forward_response(int target_socket, int client_socket);
static int connect_to_target_server(const char *hostname, int port);
static bool parse_http_request(const char *buffer, char *hostname, int *port);
static bool receive_client_request(int client_socket, char *buffer);

/**
 * @brief The entry point to handle a client requests on a pthread.
 * @param arg the client socket
 */
void handle_client_request_thread(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);
    handle_client_request(client_socket);
}

/**
 * @brief The main logic for handling a client request
 * @param client_socket
 */
static void handle_client_request(int client_socket) {
    char buffer[BUFFER_SIZE];

    // Receive the HTTP request from the client
    if (!receive_client_request(client_socket, buffer)) {
        return;
    }

    char hostname[256];
    int port;

    // Parse the HTTP request
    if (!parse_http_request(buffer, hostname, &port)) {
        return;
    }

    // Connect to the target server
    int target_socket = connect_to_target_server(hostname, port);
    if (target_socket < 0) {
        return;
    }

    // Forward the client's request to the target server
    forward_request(target_socket, buffer);

    // Receive response from target and forward to client
    forward_response(target_socket, client_socket);

    close(target_socket);
    close(client_socket);
    printf("Request completed successfully.\n");
}

/**
 * @brief Forwards the request to the target socket
 * @param target_socket
 * @param buffer
 */
static void forward_request(int target_socket, char buffer[]) {
    char method[10], url[1024], protocol[10], path[1024];
    memset(path, 0, sizeof(path));

    sscanf(buffer, "%s %s %s", method, url, protocol);

    if (strncmp(url, "http://", 7) == 0) {
        char *path_start = strchr(url + 7, '/');
        if (path_start) {
            strncpy(path, path_start, sizeof(path) - 1);
        } else {
            strcpy(path, "/");
        }
    } else {
        perror("Invalid HTTP request URL");
        return;
    }

    char modified_request[BUFFER_SIZE];
    snprintf(modified_request, sizeof(modified_request), "%s %s %s\r\n", method, path, protocol);

    char *headers = strstr(buffer, "\r\n");
    if (headers) {
        strncat(modified_request, headers + 2, sizeof(modified_request) - strlen(modified_request) - 1);
    }

    printf("%s\n", modified_request);

    ssize_t bytes_sent = send(target_socket, modified_request, strlen(modified_request), MSG_NOSIGNAL);
    if (bytes_sent == -1) {
        perror("Send to target server failed");
    }
}

static bool receive_client_request(int client_socket, char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        perror("Failed to receive data from client");
        return false;
    }
    printf("Request received from client:\n%s\n", buffer);
    return true;
}

static bool parse_http_request(const char *buffer, char *hostname, int *port) {
    char method[10], url[1024], protocol[10];
    sscanf(buffer, "%s %s %s", method, url, protocol);

    *port = 80;

    if (strncmp(url, "http://", 7) == 0) {
        if (sscanf(url + 7, "%255[^:/]:%d", hostname, port) == 2) {
            printf("Hostname: %s, Port: %d\n", hostname, *port);
        } else if (sscanf(url + 7, "%255[^/]", hostname) == 1) {
            printf("Hostname: %s, Port: %d (default)\n", hostname, *port);
        } else {
            perror("Failed to parse hostname");
            return false;
        }
    } else if (strncmp(url, "https://", 8) == 0) {
        perror("Only HTTP requests supported at this stage");
        return false;
    } else {
        fprintf(stderr, "Unexpected URL '%s'\n", url);
        fprintf(stderr, "Proxy requires absolute URL to target server.\n");
        return false;
    }

    return true;
}

/**
 * @brief Connects to the target server
 * @param hostname the target host
 * @param port the target port
 * @return the socket for the target server or -1 on failure to connect
 */
static int connect_to_target_server(const char *hostname, int port) {
    struct addrinfo hints, *res, *p;
    char port_str[6];
    int sockfd;

    snprintf(port_str, sizeof(port_str), "%d", port);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, port_str, &hints, &res) != 0) {
        perror("getaddrinfo failed");
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1) break;

        close(sockfd);
    }

    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "Failed to connect to target server\n");
        return -1;
    }

    return sockfd;
}

/**
 * @brief Receives the response from the target socket and sends it to the client socket.
 */
static void forward_response(int target_socket, int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(target_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        ssize_t bytes_sent = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sent == -1) {
            perror("Send to client failed");
            break;
        }
    }

    if (bytes_received < 0) {
        perror("Receive from target server failed");
    }
}
