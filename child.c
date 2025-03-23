#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigtstp_handler(int sig) {
    (void)sig; 
    printf("Child: SIGTSTP received. Sending sum to parent.\n");
    kill(getppid(), SIGUSR1); 
}

void sigusr2_handler(int sig) {
    (void)sig; 
    printf("Child: Received SIGUSR2 from Parent. Current sum = %d\n", 100);  
}

int main() {
    struct sigaction sa_tstp, sa_usr2;
    
    sa_tstp.sa_handler = sigtstp_handler;
    sigaction(SIGTSTP, &sa_tstp, NULL);

    sa_usr2.sa_handler = sigusr2_handler;
    sigaction(SIGUSR2, &sa_usr2, NULL);

    while (1) {
        printf("Child: Running... Use 'kill -SIGTSTP %d' to interrupt me.\n", getpid());
        sleep(3);
    }

    return 0;
}
