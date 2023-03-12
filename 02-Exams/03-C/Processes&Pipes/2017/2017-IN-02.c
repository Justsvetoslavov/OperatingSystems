#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

#define MAX_SIZE 5  // Max length for command and arguments (4 chars + null terminator)

char cmd[MAX_SIZE];  // Store the command name

int main(int argc, char* argv[]) {
    // Step 1: Determine which command to execute
    if (argc == 1) {
        // If no argument is provided, default to "echo"
        strcpy(cmd, "echo");
    } else if (argc == 2) {
        // If a command is provided, validate its length
        if (strlen(argv[1]) >= MAX_SIZE) {
            errx(1, "Invalid length of command: %s", argv[1]);
        }
        strcpy(cmd, argv[1]);
    } else {
        // More than one argument is an error
        errx(2, "Invalid argument count");
    }
    cmd[MAX_SIZE - 1] = '\0';  // Ensure null termination for safety

    // Step 2: Prepare buffers for reading words
    int read_bytes;
    char s;

    char buff1[MAX_SIZE] = {0};  // First argument buffer
    char buff2[MAX_SIZE] = {0};  // Second argument buffer
    int size1 = 0;  // Current size of buff1
    int size2 = 0;  // Current size of buff2
    bool fword = true;  // Flag: currently filling the first word
    bool sword = false; // Flag: currently filling the second word

    // Step 3: Read input character-by-character
    while ((read_bytes = read(0, &s, sizeof(char))) > 0) {
        if (s == ' ' || s == '\n') { // Delimiter found (space or newline)
            if (size1 == 0 && size2 == 0) {  
                // Ignore consecutive delimiters (empty words)
                continue;
            }

            if (sword) { 
                // We have both words, so execute the command
                buff1[size1] = '\0';
                buff2[size2] = '\0';

                pid_t wpid = fork();
                if (wpid < 0) {
                    // Fork failed, log an error and exit
                    err(5, "Fork failed");
                }

                if (wpid == 0) {
                    // Child process: Execute the command
                    if (execlp(cmd, cmd, buff1, buff2, (char*)NULL) == -1) {
                        // execlp failed, report the error
                        err(6, "Execution failed: %s", cmd);
                    }
                }

                // Parent process: Wait for the child to finish
                int status;
                if (wait(&status) == -1) {
                    // If wait fails, log a warning
                    warn("Wait failed for child process");
                } else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    // If the child process exited with an error status, log it
                    warnx("Command '%s' exited with status %d", cmd, WEXITSTATUS(status));
                }

                // Reset buffers after execution
                memset(buff1, 0, MAX_SIZE);
                memset(buff2, 0, MAX_SIZE);
                size1 = 0;
                size2 = 0;
                fword = true;
                sword = false;
            } else {
                // First word is filled, now fill the second word
                fword = false;
                sword = true;
            }
        } else {
            // Handle normal characters (not delimiters)
            if (fword) {
                // Storing in first buffer
                if (size1 < MAX_SIZE - 1) {
                    buff1[size1++] = s;
                } else {
                    errx(3, "Invalid argument: word too long");
                }
            } else if (sword) {
                // Storing in second buffer
                if (size2 < MAX_SIZE - 1) {
                    buff2[size2++] = s;
                } else {
                    errx(4, "Invalid argument: word too long");
                }
            }
        }
    }

    // Step 4: Handle errors from read()
    if (read_bytes < 0) {
        err(7, "Could not read from stdin");
    }

    // Step 5: Execute remaining command if there is one last word left
    if (size1 > 0) {
        buff1[size1] = '\0';  // Ensure null termination

        pid_t wpid = fork();
        if (wpid < 0) {
            err(8, "Fork failed");
        }

        if (wpid == 0) {
            // Execute with one argument
            if (execlp(cmd, cmd, buff1, (char*)NULL) == -1) {
                // execlp failed, report the error
                err(9, "Execution failed: %s", cmd);
            }
        }

        int status;
        if (wait(&status) == -1) {
            // If wait fails, log a warning
            warn("Wait failed for final execution");
        } else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            // If the child process exited with an error status, log it
            warnx("Command '%s' exited with status %d", cmd, WEXITSTATUS(status));
        }
    }

    return 0;
}
