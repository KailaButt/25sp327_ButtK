#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int sum = 0;

void sigtstp_handler(int sig, siginfo_t *info, void *context) {
    sum += 10;
    union sigval value;
    value.sival_int = sum;
    sigqueue(info->si_pid, SIGUSR1, value);
}

void sigusr2_handler(int sig) {
    printf("Child: Received SIGUSR2 from Parent. Current sum = %d\n", sum);
}

void sigterm_handler(int sig) {
    union sigval value;
    value.sival_int = sum;
    sigqueue(getppid(), SIGUSR1, value);
    printf("Child: Received SIGTERM, exiting...\n");
    exit(0);
}

int main(void) {
    struct sigaction sigact;

    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = sigtstp_handler;
    sigaction(SIGTSTP, &sigact, NULL);

    sigact.sa_flags = 0;
    sigact.sa_sigaction = sigusr2_handler;
    sigaction(SIGUSR2, &sigact, NULL);

    signal(SIGTERM, sigterm_handler);

    while (1) {
        sleep(1);
    }

    return 0;
}
