#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "config.h"

int parse_args(int argc, char *argv[], scan_config_t *config) {
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        return ARG_SHOW_HELP; // signal help requested
    }

    if (argc < 4) {
        return ARG_ERR_COUNT; // argument count error
    }

    char *endptr;
    config->ip = argv[1];

    config->start_port = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    config->end_port = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    config->timeout_ms = DEFAULT_TIMEOUT;

    for (int i = 4; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 >= argc) return ARG_ERR_TIMEOUT;

            long parsed_timeout = strtol(argv[i + 1], &endptr, 10);
            if (*endptr != '\0' || parsed_timeout <= 0) {
                return ARG_ERR_TIMEOUT;
            }
            config->timeout_ms = parsed_timeout;
            i++; // skip value after -t
        } else {
            return ARG_ERR_TIMEOUT;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--threads") == 0) {
            if (i + 1 < argc) {
                char *endptr;
                long val = strtol(argv[i + 1], &endptr, 10);
                if (*endptr == '\0' && val > 0 && val <= MAX_THREADS_LIMIT) {
                    config->thread_count = (int)val;
                    i++;
                } else {
                    fprintf(stderr, "Invalid number of threads: %s\n", argv[i + 1]);
                    return ARG_ERR_THREADS;
                }
            } else {
                fprintf(stderr, "Missing value for %s\n", argv[i]);
                return ARG_ERR_THREADS;
            }
        }
    }
    return ARG_SUCCESS; // success
}

int validate_port_range(int start_port, int end_port) {
    return (start_port > 0 && end_port <= MAX_PORT_NUMBER && start_port <= end_port);
}

int validate_ip(const char *ip) {
    struct in_addr addr;
    return inet_pton(AF_INET, ip, &addr) == 1;
}

void print_help(const char *prog_name, HelpMode mode) {
    if (mode == HELP_ERROR_ARGS) {
        printf("Error: Missing or invalid arguments.\n\n");
    } else if (mode == HELP_ERROR_FLAG) {
        printf("Error: Unknown or malformed option.\n\n");
    } else if (mode == HELP_REQUESTED) {
        printf("Usage: %s <ip> <start_port> <end_port>\n", prog_name);
        printf("Example: %s 127.0.0.1 20 1024\n", prog_name);
        printf("\nArguments:\n");
        printf("  <ip>            Target IPv4 address to scan\n");
        printf("  <start_port>    Starting port number (1–65535)\n");
        printf("  <end_port>      Ending port number (1–65535)\n");
        printf("\nOptions:\n");
        printf("  -t <timeout>    Set timeout in milliseconds (default: 100)\n");
        printf("  -n, --threads <num>   Set number of threads (default: 100)\n");
        printf("  -h, --help      Show this help message and exit\n");
    }
}