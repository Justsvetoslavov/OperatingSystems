#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>

void close_safe(int fd);

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "Expected 2 arguments. Usage: %s <string>", argv[0]);
    }

    int pf[2];

    if (pipe(pf) == -1) {
        err(3, "Could not create pipe");
    }

    const char* str = argv[1];
    int str_size = strlen(str);
    pid_t child = fork();

    if (child == -1) {
        err(2, "Could not fork process");
    }

    if (child == 0) {  // Child process
        close(pf[1]);  // Close the write end of the pipe

        char buf[1024];  // Read buffer
        int bytes_count;
        
        while ((bytes_count = read(pf[0], buf, sizeof(buf))) > 0) {
            if (write(1, buf, bytes_count) != bytes_count) {
                close_safe(pf[0]);
                err(4, "Error while writing to stdout");
            }
        }

        if (bytes_count == -1) {
            close_safe(pf[0]);
            err(5, "Error while reading from pipe");
        }

        close_safe(pf[0]);
        exit(0);
    }

    // Parent process
    close_safe(pf[0]);  // Close the read end of the pipe
    if (write(pf[1], str, str_size) != str_size) {
        close_safe(pf[1]);
        err(4, "Error while writing to pipe");
    }
    close_safe(pf[1]);

    // Wait for child process to finish
    wait(NULL);

    return 0;
}

void close_safe(int fd) {
    close(fd);
}
