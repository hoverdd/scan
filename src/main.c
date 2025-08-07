#include <time.h>
#include <stdio.h>

#include "scanner.h"
#include "banner.h"
#include "argc.h"


int main(int argc, char *argv[]) {
    char *ip;
    int start_port, end_port, timeout_ms;

    int parse_result = parse_args(argc, argv, &ip, &start_port, &end_port, &timeout_ms);
    if (parse_result != 0) {
        if (parse_result == ARG_ERR_COUNT) {
            fprintf(stderr, "Error: Insufficient or invalid arguments.\n");
        } else if (parse_result == ARG_ERR_PORT) {
            fprintf(stderr, "Error: Invalid port values.\n");
        } else if (parse_result == ARG_ERR_TIMEOUT) {
            fprintf(stderr, "Error: Invalid timeout or unknown option.\n");
        }
        print_help(argv[0]);
        return 1;
    }

    if (!validate_port_range(start_port, end_port)) {
        fprintf(stderr, "Invalid port range. Ports must be between 1 and %d.\n", MAX_PORT_NUMBER);
        return 1;
    }
    print_banner();
    fflush(stdout);
    
    printf("Starting scan of host %s from port %d to %d (timeout: %dms)...\n\n",
       ip, start_port, end_port, timeout_ms);
    
       clock_t start_time = clock();
    scan_ports(ip, start_port, end_port, timeout_ms);
    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nScan completed in %.2f seconds.\n", duration);

    return 0;
}
