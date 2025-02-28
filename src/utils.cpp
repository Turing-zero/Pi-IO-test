#include "utils.h"
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
volatile sig_atomic_t g_signal_flag = 1;

static void sig_handler(int signum) {
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...\n");
        g_signal_flag = 0;
    }
}

void init_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, nullptr) == -1) {
        const char err_msg[] = "sigaction failed: ";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        _exit(EXIT_FAILURE);
    }
}
