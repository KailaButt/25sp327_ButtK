#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fsuid.h>
#include <stdlib.h>

// Macro to print RUID, EUID, SUID
#define print_ids() { \
    uid_t ruid, euid, suid; \
    if (getresuid(&ruid, &euid, &suid) == -1) { \
        perror("getresuid failed"); \
        exit(EXIT_FAILURE); \
    } \
    printf("RUID=%d, EUID=%d, SUID=%d\n", ruid, euid, suid); \
}

int main() {
    // Print initial IDs
    printf("t0: Running with root privileges: ");
    print_ids();

    // Temporarily drop privileges
    if (seteuid(getuid()) == -1) {
        perror("seteuid failed");
        exit(EXIT_FAILURE);
    }
    printf("t1: Dropped privileges temporarily: ");
    print_ids();

    // Restore root privileges
    if (seteuid(0) == -1) {
        perror("seteuid failed");
        exit(EXIT_FAILURE);
    }
    printf("t2: Restored privileges: ");
    print_ids();

    // Drop privileges again
    if (seteuid(getuid()) == -1) {
        perror("seteuid failed");
        exit(EXIT_FAILURE);
    }
    printf("t3: Dropped privileges again: ");
    print_ids();

    // Permanently drop privileges using setresuid
    if (setresuid(getuid(), getuid(), getuid()) == -1) {
        perror("setresuid failed");
        exit(EXIT_FAILURE);
    }
    printf("t4: Permanently dropping privileges...\n");
    printf("t4: After permanent drop: ");
    print_ids();

    // Attempt to restore privileges
    if (seteuid(0) == -1) {
        perror("seteuid failed");
        printf("t5: Failed to restore privileges: Operation not permitted\n");
    } else {
        printf("t5: Restored privileges: ");
        print_ids();
    }

    return EXIT_SUCCESS;
}
