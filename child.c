#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

volatile sig_atomic_t child_sum = 0;

static void sigusr2_handler(int sig) {
    UNUSED(sig);
    printf("Child: Received SIGUSR2 from Parent.\n");
}

static void sigterm_handler(int sig) {
    UNUSED(sig);
    printf("Child: Received SIGTERM. Exiting...\n");
    exit(0);
}

static void sigtstp_handler(int sig) {
    UNUSED(sig);
    static int sum = 0;
    sum += 5;
    union sigval value;
    value.sival_int = sum;
    sigqueue(getppid(), SIGUSR1, value);
    printf("Child: Sent SIGUSR1 to Parent with sum = %d\n", sum);
}

int main(void) {
    struct sigaction sigact_usr2, sigact_term, sigact_tstp;

    memset(&sigact_usr2, 0, sizeof(sigact_usr2));
    sigact_usr2.sa_handler = sigusr2_handler;
    sigaction(SIGUSR2, &sigact_usr2, NULL);

    memset(&sigact_term, 0, sizeof(sigact_term));
    sigact_term.sa_handler = sigterm_handler;
    sigaction(SIGTERM, &sigact_term, NULL);

    memset(&sigact_tstp, 0, sizeof(sigact_tstp));
    sigact_tstp.sa_handler = sigtstp_handler;
    sigaction(SIGTSTP, &sigact_tstp, NULL);

    while (1) {
        sleep(1);
    }

    return 0;
}
