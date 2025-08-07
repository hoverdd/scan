#include "argc.h"

void print_help(const char *prog_name) {
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

int parse_args(
    int argc, char *argv[], char **ip,
    int *start_port, int *end_port, int *timeout_ms
) {
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        return ARG_SHOW_HELP; // signal help requested
    }

    if (argc < 4) {
        return ARG_ERR_COUNT; // argument count error
    }

    char *endptr;

    *ip = argv[1];

    *start_port = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    *end_port = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    *timeout_ms = DEFAULT_TIMEOUT;

    for (int i = 4; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {
                *timeout_ms = strtol(argv[i + 1], &endptr, 10);
                if (*endptr != '\0') {
                    return ARG_ERR_TIMEOUT;
                }
                i++;
            } else {
                return ARG_ERR_TIMEOUT;
            }
        } else {
            return ARG_ERR_TIMEOUT;
        }
    }
    return ARG_SUCCESS; // success
}

int validate_port_range(int start_port, int end_port) {
    return (start_port >= 1 && start_port <= MAX_PORT_NUMBER &&
            end_port >= 1 && end_port <= MAX_PORT_NUMBER &&
            start_port <= end_port);
}
