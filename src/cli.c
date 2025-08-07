#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "cli.h"

int parse_args(int argc, char *argv[], scan_config_t *config) {
    config->timeout_ms = 100;
    config->thread_count = 100;

    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"threads", required_argument, 0, 'n'},
        {"timeout", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "ht:n:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                return ARG_SHOW_HELP;
            case 't': {
                char *endptr;
                long parsed_timeout = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || parsed_timeout <= 0) {
                    return ARG_ERR_TIMEOUT;
                }
                config->timeout_ms = parsed_timeout;
                break;
            }
            case 'n': {
                char *endptr;
                long val = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || val <= 0 || val > MAX_THREADS_LIMIT) {
                    fprintf(stderr, "Invalid number of threads: %d (must be 1 to %d)\n",
                        config->thread_count, MAX_THREADS_LIMIT);
                    return ARG_ERR_THREADS;
                }
                config->thread_count = (int)val;
                break;
            }
            case '?':
                return ARG_ERR_TIMEOUT;
            default:
                return ARG_ERR_TIMEOUT;
        }
    }

    if (argc - optind < 3) {
        return ARG_ERR_COUNT;
    }

    char *endptr;

    config->ip = argv[optind];

    config->start_port = strtol(argv[optind + 1], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    config->end_port = strtol(argv[optind + 2], &endptr, 10);
    if (*endptr != '\0') {
        return ARG_ERR_PORT;
    }

    return ARG_SUCCESS; // success
}