#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <err.h>

void execute_command(const char*);

void write_pid(pid_t pid) {
    // Write the pid as bytes (it's a number, so we can directly write it)
    if (write(1, &pid, sizeof(pid)) != sizeof(pid)) {
        err(6, "Failed to write PID to stdout");
    }
    
    // Write a newline character
    char newline = '\n';
    if (write(1, &newline, 1) != 1) {
        err(6, "Failed to write newline to stdout");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Error: Invalid number of arguments: %d", argc - 1);
    }

    execute_command(argv[1]);
    execute_command(argv[2]);

    int status1, status2;
    const pid_t first = wait(&status1);
    if (first == -1) {
        err(4, "Could not wait for any process");
    }

    if (WIFEXITED(status1) && WEXITSTATUS(status1)) {
        write_pid(first);
    } else {
        const pid_t second = wait(&status2);
        if (second == -1) {
            err(5, "Could not wait for second process");
        }
        if (WIFEXITED(status2) && !WEXITSTATUS(status2)) {
            write_pid(second);
        } else {
            write_pid(-1);
        }
    }
}

void execute_command(const char* cmd) {
    const pid_t pid = fork();

    if (pid == -1) {
        err(2, "Could not fork");
    }

    if (pid == 0) {
        execlp(cmd, cmd, (char *)NULL);
        err(3, "Could not execlp %s", cmd);
    }
}
