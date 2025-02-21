#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_host_name();
void get_cpu_info();
void get_kernel_version();
void get_uptime();
void get_cpu_usage();
void get_memory_info();

int main() {
    printf("----------------------------------------------------------------------------\n");
    printf("                             System Information\n");
    printf("----------------------------------------------------------------------------\n");

    get_host_name();
    get_cpu_info();
    get_kernel_version();
    get_uptime();
    get_cpu_usage();
    get_memory_info();

    printf("----------------------------------------------------------------------------\n");
    return 0;
}

// host name
void get_host_name() {
    char host_name[256];
    FILE *file = fopen("/proc/sys/kernel/hostname", "r");
    if (file == NULL) {
        perror("Failed to read host name");
        exit(1);
    }
    fgets(host_name, sizeof(host_name), file);
    fclose(file);
    printf("Current Host Name:           %s", host_name);
}

// CPU info
void get_cpu_info() {
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Failed to read CPU info");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "model name", 10) == 0) {
            printf("CPU Model:                   %s", line + 13);
        }
        if (strncmp(line, "processor", 9) == 0) {
            printf("CPU Cores:                   %d\n", atoi(line + 11));
            break;
        }
    }
    fclose(file);
}

// kernel version
void get_kernel_version() {
    char version[256];
    FILE *file = fopen("/proc/version", "r");
    if (file == NULL) {
        perror("Failed to read kernel version");
        exit(1);
    }
    fgets(version, sizeof(version), file);
    fclose(file);
    printf("Kernel Version               %s", version);
}

// uptime, formated it as days, hours, minutes, seconds
void get_uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (file == NULL) {
        perror("Failed to read uptime");
        exit(1);
    }

    float uptime_sec;
    fscanf(file, "%f", &uptime_sec);
    fclose(file);

    int days = uptime_sec / (24 * 3600);
    int hours = (uptime_sec - (days * 24 * 3600)) / 3600;
    int minutes = (uptime_sec - (days * 24 * 3600) - (hours * 3600)) / 60;
    int seconds = uptime_sec - (days * 24 * 3600) - (hours * 3600) - (minutes * 60);

    printf("Uptime:                      %d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
}

//  time spent in user, system, and idle mode from /proc/stat
void get_cpu_usage() {
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Failed to read CPU usage");
        exit(1);
    }

    char line[256];
    long user, system, idle;
    fgets(line, sizeof(line), file);
    sscanf(line, "cpu  %ld %*d %ld %ld", &user, &system, &idle);
    fclose(file);

    printf("Time Spent in User Mode:     %ld ms\n", user);
    printf("Time Spent in System Mode:   %ld ms\n", system);
    printf("Time Spent in Idle Mode:     %ld ms\n", idle);
}

//  memory information
void get_memory_info() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Failed to read memory info");
        exit(1);
    }

    char line[256];
    long mem_total, mem_free;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal", 8) == 0) {
            sscanf(line, "MemTotal: %ld kB", &mem_total);
        }
        if (strncmp(line, "MemFree", 7) == 0) {
            sscanf(line, "MemFree: %ld kB", &mem_free);
        }
    }
    fclose(file);

    printf("Memory Configured:           %ld kB\n", mem_total);
    printf("Memory Available:            %ld kB\n", mem_free);
}
