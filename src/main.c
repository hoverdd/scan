#include <time.h>
#include <stdio.h>

#include "scanner.h"
#include "banner.h"
#include "config.h"


int main(int argc, char *argv[]) {
    scan_config_t config;
    int parse_result = parse_args(argc, argv, &config);
    
    if (parse_result == ARG_SHOW_HELP) {
        print_help(argv[0], HELP_REQUESTED);
        return 0;
    } else if (parse_result != ARG_SUCCESS) {
        if (parse_result == ARG_ERR_COUNT) {
            print_help(argv[0], HELP_ERROR_ARGS);
        } else if (parse_result == ARG_ERR_PORT) {
            print_help(argv[0], HELP_ERROR_ARGS);
        } else if (parse_result == ARG_ERR_TIMEOUT) {
            print_help(argv[0], HELP_ERROR_FLAG);
        }
        return 1;
    }

    if (!validate_ip(config.ip)) {
        fprintf(stderr, "Invalid IP address: %s\n", config.ip);
        return 1;
    }
    
    if (!validate_port_range(config.start_port, config.end_port)) {
        fprintf(stderr, "Invalid port range. Ports must be between 1 and %d.\n", MAX_PORT_NUMBER);
        return 1;
    }

    print_banner();
    fflush(stdout);
    
    printf("Starting scan of host %s from port %d to %d (timeout: %dms)...\n\n",
       config.ip, config.start_port, config.end_port, config.timeout_ms);
    
    printf("config.thread_count=%d\nMAX_THREADS_LIMIT=%d\n", config.thread_count, MAX_THREADS_LIMIT);
    clock_t start_time = clock();
    scan_args_t args = {
        .ip = config.ip,
        .timeout_ms = config.timeout_ms,
        .start_port = config.start_port,
        .end_port = config.end_port,
        .max_threads = 100 
        // before: .max_threads = config.thread_count > 0 ? config.thread_count : MAX_THREADS_LIMIT
    }; 
    scan_ports_threaded(&args);
    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nScan completed in %.2f seconds.\n", duration);

    return 0;
}
