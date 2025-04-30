#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "tasks.h"

void example_task(void* arg) {
    int* val = (int*)arg;
    FILE* log_file = fopen("task_log.txt", "a");
    if (!log_file) {
        perror("fopen");
        return;
    }

    // Get time
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", tm_info);

    // Get thread ID
    pid_t tid = syscall(SYS_gettid);

    // Choose random task type
    int task_type = rand() % 3;

    fprintf(log_file, "%s [Thread %d] Log message %d\n", timestamp, tid, *val);

    if (task_type == 0) {
        int x = rand() % 1000;
        fprintf(log_file, "%s [Thread %d] Sum of numbers < %d is %d\n",
                timestamp, tid, x, (x * (x - 1)) / 2);
    } else if (task_type == 1) {
        int delay_ms = (rand() % 200) + 50;
        usleep(delay_ms * 1000);
        fprintf(log_file, "%s [Thread %d] Simulated delay of 0 seconds and %d milliseconds.\n",
                timestamp, tid, delay_ms);
    } else {
        fprintf(log_file, "%s [Thread %d] Log message %d\n", timestamp, tid, *val);
    }

    fclose(log_file);
}
