#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "scanner.h"

int scan_single_port(const char *ip, int port, int timeout_ms) {
    struct sockaddr_in target;
    memset(&target, 0, sizeof(target)); // zero out the struct
    target.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &target.sin_addr);


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 1;

    target.sin_port = htons(port);

    // setup timeout
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    int res = connect(sock, (struct sockaddr *)&target, sizeof(target));
    if (res == 0) {
        printf("Port %d is OPEN\n", port);
    }
    close(sock);
    return 0;
}

void *thread_scan(void *arg) {
    scan_args_t *args = (scan_args_t *)arg;

    while (1) {
        pthread_mutex_lock(&args->lock);
        if (args->next_port > args->end_port) {
            pthread_mutex_unlock(&args->lock);
            break;
        } 
        int port = args->next_port++;
        pthread_mutex_unlock(&args->lock);
        
        // printf("Scanning port %d\n", port);
        scan_single_port(args->ip, port, args->timeout_ms); 
    }
    return NULL;
}

int scan_ports_threaded(scan_args_t *args) {
    pthread_t threads[args->max_threads];

    pthread_mutex_init(&args->lock, NULL);
    args->next_port = args->start_port;

    for (int i = 0; i < args->max_threads; i++) {
        pthread_create(&threads[i], NULL, thread_scan, args);
    }

    for (int i = 0; i < args->max_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&args->lock);
    
    return 0;
}