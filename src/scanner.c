#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#include "scanner.h"


int scan_single_port(const char *ip, int port, int timeout_ms) {
    int sockfd;
    struct sockaddr_in target;
    struct timeval timeout;

    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return 0;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));

    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target.sin_addr);

    int result = connect(sockfd, (struct sockaddr *)&target, sizeof(target));
    close(sockfd);

    return result == 0 ? 1 : 0;
}


void queue_init(port_queue_t *q, int capacity) {
    q->buffer = malloc(sizeof(int) * capacity);
    if (!q->buffer) {
       fprintf(stderr, "Failed to allocate memory for port queue\n");
        exit(EXIT_FAILURE); 
    }
    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->closed = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->cond, NULL);
}

void queue_destroy(port_queue_t *q) {
    free(q->buffer);
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->cond);
}

void queue_push(port_queue_t *q, int port) {
    pthread_mutex_lock(&q->lock);
    if (q->size < q->capacity) {
        q->buffer[q->rear] = port;
        q->rear = (q->rear + 1) % q->capacity;
        q->size++;
        pthread_cond_signal(&q->cond);
    }
    pthread_mutex_unlock(&q->lock);
}

int queue_pop(port_queue_t *q, int *port) {
    pthread_mutex_lock(&q->lock);
    while (q->size == 0 && !q->closed) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    if (q->size == 0 && q->closed) {
        pthread_mutex_unlock(&q->lock);
        return 0;
    }
    *port = q->buffer[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    pthread_mutex_unlock(&q->lock);
    return 1;
}

void *thread_scan(void *arg) {
    scan_args_t *args = (scan_args_t *)arg;
    int port;

    while (queue_pop(&args->queue, &port)) {
        int result = scan_single_port(args->ip, port, args->timeout_ms);
        pthread_mutex_lock(&args->lock);
        if (result == 1) {
            args->open_ports++;
            printf("Port %d is OPEN\n", port);
        } else {
            args->closed_ports++;
        }
        pthread_mutex_unlock(&args->lock);
    }
    return NULL;
}

int scan_ports_threaded(scan_args_t *args) {
    pthread_t threads[args->max_threads];
    memset(threads, 0, sizeof(threads));

    pthread_mutex_init(&args->lock, NULL);

    args->open_ports = 0;
    args->closed_ports = 0;

    int port_count = args->end_port - args->start_port + 1;
    queue_init(&args->queue, port_count);

    for (int port = args->start_port; port <= args->end_port; port++) {
        queue_push(&args->queue, port);
    }

    for (int i = 0; i < args->max_threads; i++) {
        int rc = pthread_create(&threads[i], NULL, thread_scan, args);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d: %s\n", i, strerror(rc));
            threads[i] = 0;
        }
    }

    pthread_mutex_lock(&args->queue.lock);
    args->queue.closed = 1;
    pthread_cond_broadcast(&args->queue.cond);
    pthread_mutex_unlock(&args->queue.lock);

    for (int i = 0; i < args->max_threads; i++) {
        if (threads[i] != 0) {
            pthread_join(threads[i], NULL);
        }
    }

    queue_destroy(&args->queue);
    pthread_mutex_destroy(&args->lock);
    
    printf("\nScan completed: %d open, %d closed\n", args->open_ports, args->closed_ports);
    return 0;
}