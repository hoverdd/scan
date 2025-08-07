#pragma once
#define MAX_PROCESSES 128

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

void scan_ports(const char *ip, int start_port, int end_port, int timeout_ms);
int scan_single_port(const char *ip, int port, int timeout_ms);