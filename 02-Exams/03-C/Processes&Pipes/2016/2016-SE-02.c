
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <err.h>

#define MAX_SIZE 32

int main(void) {
        char buff[MAX_SIZE];
        memset(buff, 0, MAX_SIZE); // Optional
        while(true) {
                dprintf(1, "command prompt:");
                int read_bytes;
                if(MAX_SIZE >= (read_bytes = read(0, buff, MAX_SIZE))) {
                        buff[read_bytes - 1] = '\0';
                        if(0 == strcmp("exit", buff)) {
                                dprintf(1, "You have entered command: exit\nExiting...\n");
                                exit(0);
                        }

                        pid_t wpid = fork();
                        if(wpid < 0) {
                                err(2, "Could not fork");
                        }
                        if(0 == wpid) {
                                char cmd[MAX_SIZE + 5];
                                strcpy(cmd, "/bin/");
                                strcat(cmd, buff);
                                if(execl(cmd, cmd, (char*)NULL) < 0) {
                                        err(3, "Could not execlp command: %s", buff);
                                }
                        }

                        int status;
                        if(wait(&status) < 0) {
                                err(4, "Could not wait for child process");
                        }
                        if(!WIFEXITED(status)) {
                                err(5, "Child process did not terminate normally");
                        }
                        if(WEXITSTATUS(status)) {
                                warnx("Command %s failed with exit status %d", buff, status);
                        }
                }
                if(read_bytes < 0) {
                        err(1, "Could not read from stdout");
                }
        }
}
