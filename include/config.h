#pragma once

#include <pthread.h>

#define SCANNER_VERSION "0.1"
#define MAX_THREADS_LIMIT 100
#define MAX_PORT_NUMBER 65535
#define DEFAULT_TIMEOUT 100
#define ARG_SUCCESS      0
#define ARG_SHOW_HELP    -1
#define ARG_ERR_COUNT    1
#define ARG_ERR_PORT     2
#define ARG_ERR_TIMEOUT  3
#define ARG_ERR_THREADS  4
#define ARG_ERR_IP       5

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
    int next_port;
    int max_threads;
    pthread_mutex_t lock; // Mutex to protect access to shared next_port
} scan_args_t;

typedef enum {
    HELP_REQUESTED,
    HELP_ERROR_ARGS,
    HELP_ERROR_FLAG
} HelpMode;

/**
 * Validates that the start and end ports are within valid ranges and correctly ordered.
 * @param start_port The starting port number.
 * @param end_port The ending port number.
 * @return Returns 1 if the port range is valid; otherwise, returns 0.
 */
int validate_port_range(int start_port, int end_port);

/**
 * Validates whether the given string is a valid IPv4 address.
 *
 * @param ip The IP address string to validate.
 * @return Returns 1 if the IP address is valid, 0 otherwise.
 */
int validate_ip(const char *ip);