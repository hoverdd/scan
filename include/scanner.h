#pragma once

#include "config.h"

/**
 * Scans a single TCP port.
 * @param ip Target IP address.
 * @param port Port number to scan.
 * @param timeout_ms Timeout in milliseconds.
 * @return 1 if port is open, 0 otherwise.
 */
int scan_single_port(const char *ip, int port, int timeout_ms);

/**
 * Scans a range of ports concurrently using multiple threads.
 * @param args Pointer to the scanning configuration and state.
 * @return 0 on success, non-zero on error.
 */
int scan_ports_threaded(scan_args_t *args);

/**
 * Thread worker function that scans ports from the shared range.
 * Each thread calls this function to scan ports until all are done.
 * @param arg Pointer to scan_args_t structure.
 * @return NULL
 */
void *thread_scan(void *arg);