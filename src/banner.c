#include "banner.h"

const char *get_current_time_string() {
    static char time_buffer[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return time_buffer;
}

void print_banner() {
    printf("CTools Scanner (v%s)\n", SCANNER_VERSION);
    printf("Started at %s\n", get_current_time_string());
}