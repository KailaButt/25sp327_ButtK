#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

volatile sig_atomic_t sum = 0;
volatile sig_atomic_t sigusr2_received = 0;
volatile sig_atomic_t sigtstp_received = 0;
volatile sig_atomic_t sigterm_received = 0;

static void signal_handler(int sig, siginfo_t *info, void *context) {
    UNUSED(info); UNUSED(context);

    if (sig == SIGUSR2) {
        sigusr2_received = 1;
    } else if (sig == SIGTSTP) {
        sigtstp_received = 1;
    } else if (sig == SIGTERM) {
        sigterm_received = 1;
    }
}

int main(void) {
    struct sigaction sigact;
    
    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = signal_handler;
    
    sigaction(SIGTSTP, &sigact, NULL);
    sigaction(SIGUSR2, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);

    while (1) {
        sum += 10;
        printf("Child sum: %d\n", sum);

        if (sigtstp_received) {
            sigqueue(getppid(), SIGUSR1, (union sigval){ .sival_int = sum });
            printf("Child: SIGTSTP received, sending sum (%d) to parent.\n", sum);
            sigtstp_received = 0;
        }

        if (sigusr2_received) {
            printf("Child: Received SIGUSR2 from parent. Current sum = %d\n", sum);
            sigusr2_received = 0;
        }

        if (sigterm_received) {
            printf("Child: Received SIGTERM, exiting...\n");
            exit(0);
        }

        sleep(1);
    }

    return 0;
}
