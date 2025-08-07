#pragma once

#define SCANNER_VERSION "0.1"
#define MAX_PROCESSES 128
#define MAX_PORT_NUMBER 65535
#define DEFAULT_TIMEOUT 100
#define ARG_SUCCESS      0
#define ARG_SHOW_HELP    -1
#define ARG_ERR_COUNT    1
#define ARG_ERR_PORT     2
#define ARG_ERR_TIMEOUT  3

typedef struct {
    const char *ip;
    int start_port;
    int end_port;
    int timeout_ms;
} scan_config_t;

typedef enum {
    HELP_REQUESTED,
    HELP_ERROR_ARGS,
    HELP_ERROR_FLAG
} HelpMode;

/**
 * Prints usage/help message to stdout.
 * @param prog_name Name of the executable (usually argv[0])
 */
void print_help(const char *prog_name, HelpMode mode);

/**
 * Validates that the start and end ports are within valid ranges and correctly ordered.
 * @param start_port The starting port number.
 * @param end_port The ending port number.
 * @return Returns 1 if the port range is valid; otherwise, returns 0.
 */
int validate_port_range(int start_port, int end_port);


/**
 * Parses command-line arguments and fills the given scan_config_t structure.
 * Recognizes IP address, start and end ports, optional timeout, and help flags.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @param config Pointer to scan_config_t structure to fill with parsed values.
 * @return Returns ARG_SUCCESS on success,
 *         ARG_SHOW_HELP if help was requested,
 *         or an error code (ARG_ERR_COUNT, ARG_ERR_PORT, ARG_ERR_TIMEOUT) on failure.
 */
int parse_args(int argc, char *argv[], scan_config_t *config);