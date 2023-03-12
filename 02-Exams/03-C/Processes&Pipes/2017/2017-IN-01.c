#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <err.h>

#define NUM_PIPES 4  // Number of pipes needed for the 5 commands

int pfd[NUM_PIPES][2];

void create_pipe(int idx) {
    if (pipe(pfd[idx]) < 0) {
        err(1, "Could not create pipe %d", idx);
    }
}

pid_t execute_command(const char *cmd, char *const argv[], int in_fd, int out_fd, int close_fds[NUM_PIPES][2]) {
    pid_t pid = fork();
    if (pid < 0) {
        err(2, "Could not fork for %s", cmd);
    }

    if (pid == 0) { // Child process
        if (in_fd != -1) {
            if (dup2(in_fd, 0) == -1) {  // Redirect stdin
                err(3, "dup2 failed for stdin in %s", cmd);
            }
        }
        if (out_fd != -1) {
            if (dup2(out_fd, 1) == -1) { // Redirect stdout
                err(3, "dup2 failed for stdout in %s", cmd);
            }
        }

        // Close all pipes in the child process
        for (int i = 0; i < NUM_PIPES; i++) {
            close(close_fds[i][0]);
            close(close_fds[i][1]);
        }

        execvp(cmd, argv);
        err(4, "execvp failed for %s", cmd);  // If execvp fails, report error and exit
    }
    
    return pid; // Parent process returns child's PID
}

int main(void) {
    // Step 1: Create necessary pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        create_pipe(i);
    }

    // Step 2: Define the commands
    char *cmd1[] = {"cat", "/etc/passwd", NULL};
    char *cmd2[] = {"cut", "-d:", "-f7", NULL};
    char *cmd3[] = {"sort", NULL};
    char *cmd4[] = {"uniq", "-c", NULL};
    char *cmd5[] = {"sort", "-n", NULL};

    pid_t pids[5];

    // Step 3: Create child processes and link them with pipes
    pids[0] = execute_command("cat", cmd1, -1, pfd[0][1], pfd);
    close(pfd[0][1]); // Parent closes the write end of first pipe

    pids[1] = execute_command("cut", cmd2, pfd[0][0], pfd[1][1], pfd);
    close(pfd[0][0]); // Parent closes read end of first pipe
    close(pfd[1][1]); // Parent closes write end of second pipe

    pids[2] = execute_command("sort", cmd3, pfd[1][0], pfd[2][1], pfd);
    close(pfd[1][0]); // Parent closes read end of second pipe
    close(pfd[2][1]); // Parent closes write end of third pipe

    pids[3] = execute_command("uniq", cmd4, pfd[2][0], pfd[3][1], pfd);
    close(pfd[2][0]); // Parent closes read end of third pipe
    close(pfd[3][1]); // Parent closes write end of fourth pipe

    pids[4] = execute_command("sort", cmd5, pfd[3][0], -1, pfd);
    close(pfd[3][0]); // Parent closes read end of fourth pipe

    // Step 4: Wait for all child processes individually
    for (int i = 0; i < 5; i++) {
        int status;
        if (wait(&status) == -1) {
            err(5, "Error waiting for child process %d", i);
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            errx(6, "Child process %d failed with exit code %d", i, WEXITSTATUS(status));
        }
    }

    return 0;
}
