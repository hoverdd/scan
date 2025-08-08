#pragma once

#include <pthread.h>

typedef struct {
    int *buffer;
    int capacity;
    int front;
    int rear;
    int size;
    int closed;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} port_queue_t;

void queue_init(port_queue_t *queue, int capacity);

void queue_destroy(port_queue_t *queue);

void queue_push(port_queue_t *queue, int port);

int queue_pop(port_queue_t *queue, int *port);

void queue_close(port_queue_t *queue);

typedef struct {
    const char *ip;
    int start_port;
    int end_port;
    int timeout_ms;
    int thread_count;
} scan_config_t;

typedef struct {
    const char *ip;
    int timeout_ms;
    int start_port;
    int end_port;
    int max_threads;
    int open_ports;
    int closed_ports;
    pthread_mutex_t lock; // Mutex to protect access to shared open and closed ports
    port_queue_t queue;
} scan_args_t;

/**
 * Scans a single TCP port.
 * @param ip Target IP address.
 * @param port Port number to scan.
 * @param timeout_ms Timeout in milliseconds.
 * @return 1 if port is open, 0 otherwise.
 */
int scan_single_port(const char *ip, int port, int timeout_ms);

/**
 * Scans a range of ports concurrently using multiple threads.
 * @param args Pointer to the scanning configuration and state.
 * @return 0 on success, non-zero on error.
 */
int scan_ports_threaded(scan_args_t *args);

/**
 * Thread worker function that scans ports from the shared range.
 * Each thread calls this function to scan ports until all are done.
 * @param arg Pointer to scan_args_t structure.
 * @return NULL
 */
void *thread_scan(void *arg);