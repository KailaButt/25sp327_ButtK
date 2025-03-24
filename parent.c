#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

volatile sig_atomic_t child_sigusr1 = 0;
pid_t child_pid = -1;

static void signal_handler(int sig, siginfo_t *info, void *context) {
    UNUSED(context);
    if (sig == SIGUSR1) {
        int sum = info->si_value.sival_int;
        printf("Parent: Received SIGUSR1 from Child (PID: %d) with sum = %d\n", info->si_pid, sum);
        child_sigusr1 = 1;
        child_pid = info->si_pid;
    }
}

static void sigchld_handler(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG); 
    if (pid > 0) {
        printf("Parent: Child (PID: %d) has terminated. Exiting.\n", pid);
        exit(0);
    }
}

int main(void) {
    struct sigaction sigact_usr1, sigact_chld;

    memset(&sigact_usr1, 0, sizeof(sigact_usr1));
    sigact_usr1.sa_sigaction = signal_handler;
    sigact_usr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sigact_usr1, NULL);

    memset(&sigact_chld, 0, sizeof(sigact_chld));
    sigact_chld.sa_handler = sigchld_handler;
    sigaction(SIGCHLD, &sigact_chld, NULL);

    child_pid = fork();
    if (child_pid == 0) {
        execl("./child", "child", (char *)NULL);
        perror("execl failed");
        exit(1);
    } else if (child_pid > 0) {
        printf("Parent: fork-exec successful. Child PID = %d\n", child_pid);

        while (1) {
            sleep(5);
            if (child_sigusr1 == 1) {
                printf("Parent: Sending SIGUSR2 to Child (PID: %d)\n", child_pid);
                kill(child_pid, SIGUSR2);
                child_sigusr1 = 0;  
            }
        }
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
