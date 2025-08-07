#include "scanner.h"

int scan_single_port(const char *ip, int port, int timeout_ms) {
    struct sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;

    if (inet_pton(AF_INET, ip, &target.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 1;

    target.sin_port = htons(port);

    // setup timeout
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    int res = connect(sock, (struct sockaddr *)&target, sizeof(target));
    if (res == 0) {
        printf("Port %d is OPEN\n", port);
    }
    close(sock);
    return 0;
}

void scan_ports(const char *ip, int start_port, int end_port, int timeout_ms) {
    int active_procs = 0;

    for (int port = start_port; port <= end_port; port++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            scan_single_port(ip, port, timeout_ms);
            fflush(stdout);
            exit(0);
        } else if (pid > 0) {
            active_procs++;

            if (active_procs >= MAX_PROCESSES) {
                wait(NULL); // reap one child
                active_procs--;
            }
        } else {
            perror("fork failed");
        }
    }

    int status;
    pid_t wpid;

    while ((wpid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "Child process exited with code %d\n", exit_status);
            }
        } else if (WIFSIGNALED(status)) {
            int term_sig = WTERMSIG(status);
            fprintf(stderr, "Child process %d terminated by signal %d\n", wpid, term_sig);
        }
    }
    if (wpid == -1 && errno != ECHILD) {
        perror("waitpid failed");
    }
}