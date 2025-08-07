#include <stdio.h>
#include <arpa/inet.h>

#include "config.h"

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
        printf("  -t, --timeout <ms>    Set timeout in milliseconds (default: 100)\n");
        printf("  -n, --threads <num>   Set number of threads (default: 100)\n");
        printf("  -h, --help      Show this help message and exit\n");
    }
}