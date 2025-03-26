#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

volatile sig_atomic_t sum_received = 0;
volatile sig_atomic_t child_pid = 0;
int sum = 0;

static void signal_handler(int sig, siginfo_t *info, void *context);

static void signal_handler(int sig, siginfo_t *info, void *context) {
    UNUSED(context);
    if (sig == SIGUSR1) {  
        sum = info->si_value.sival_int;
        sum_received = 1;
        child_pid = info->si_pid;
    } else if (sig == SIGCHLD) {  
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0) {
            printf("Parent: Child process %d terminated.\n", pid);
            exit(0);  
        }
    }
}

int main(void) {
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_sigaction = signal_handler;
    sigact.sa_flags = SA_SIGINFO | SA_NOCLDSTOP; 
   
    sigaction(SIGUSR1, &sigact, NULL);
    sigaction(SIGCHLD, &sigact, NULL);  

    pid_t pid = fork();

    if (pid == 0) {  
        execl("./child", "child", NULL);
    } else if (pid > 0) {  
        printf("Parent: fork-exec successful. Child pid (%d)\n", pid);
        while (1) {
            sleep(5);
            if (sum_received) {
                printf("Parent: Received SIGUSR1 from Child (PID: %d). Sum = %d\n", child_pid, sum);
                kill(child_pid, SIGUSR2);  
                sum_received = 0;
            }
        }
    } else {
        perror("Fork failed");
        exit(1);
    }
}
