#pragma once

#include "cli.h"


/**
 * Prints usage/help message to stdout.
 * @param prog_name Name of the executable (usually argv[0])
 */
void print_help(const char *prog_name, HelpMode mode);

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