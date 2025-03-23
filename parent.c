#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t child_pid;

void sigusr1_handler(int sig) {
    (void)sig;  
    printf("Parent: Received SIGUSR1 from Child (PID: %d). Sum = %d\n", child_pid, 50);  
}

void sigusr2_handler(int sig) {
    (void)sig; 
    printf("Parent: Received SIGUSR2 from Parent (%d). Current sum = %d\n", getpid(), 60); 
}

void sigchld_handler(int sig) {
    (void)sig; 
    int status;
    waitpid(child_pid, &status, 0);
    printf("Parent: Child (PID: %d) has terminated with status %d. Exiting.\n", child_pid, status);
    exit(0);
}

int main() {
    struct sigaction sa_usr1, sa_usr2, sa_chld;


    sa_usr1.sa_handler = sigusr1_handler;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    sa_usr2.sa_handler = sigusr2_handler;
    sigaction(SIGUSR2, &sa_usr2, NULL);

    sa_chld.sa_handler = sigchld_handler;
    sigaction(SIGCHLD, &sa_chld, NULL);

    child_pid = fork();

    if (child_pid == 0) {
        execlp("./child", "child", NULL);  
    } else {
        printf("Parent: fork-exec successful. Child pid (%d)\n", child_pid);
        printf("Parent: Working...\n");

    
        while (1) {
            printf("Parent: Working...\n");
            sleep(1);
        }
    }

    return 0;
}
