#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    return ARG_SUCCESS; // success
}

int validate_port_range(int start_port, int end_port) {
    return (start_port > 0 && end_port <= MAX_PORT_NUMBER && start_port <= end_port);
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
        printf("  -t <timeout>    Set timeout in milliseconds (default: 50)\n");
        printf("  -h, --help      Show this help message and exit\n");
    }
}