#pragma once

#include "config.h"

/**
 * Scans ports in the range specified in config.
 * @param config Pointer to scan configuration struct.
 */
void scan_ports(scan_config_t *config);

/**
 * Scans a single TCP port.
 * @param ip Target IP address.
 * @param port Port number to scan.
 * @param timeout_ms Timeout in milliseconds.
 * @return 1 if port is open, 0 otherwise.
 */
int scan_single_port(const char *ip, int port, int timeout_ms);