#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PORT_NUMBER 65535
#define DEFAULT_TIMEOUT 100
#define ARG_SUCCESS      0
#define ARG_SHOW_HELP    -1
#define ARG_ERR_COUNT    1
#define ARG_ERR_PORT     2
#define ARG_ERR_TIMEOUT  3

void print_help(const char *prog_name);
int parse_args(
    int argc, char *argv[], char **ip,
    int *start_port, int *end_port, int *timeout_ms
);
int validate_port_range(int start_port, int end_port);