#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>

void execute_process(const char*);

int main(int argc, char* argv[]) {
        if(argc < 2) {
                errx(1, "Error: Invalid number of arguments: %d", argc - 1);
        }

        int status;
        int successful = 0;
        int unsuccessful = 0;
        for(int i = 1; i < argc; ++i) {
                execute_process(argv[i]);

                if(wait(&status) == -1) {
                        err(4, "Could not wait for child");
                }

                if(!WIFEXITED(status)) {
                        ++unsuccessful;
                } else {
                        if(WEXITSTATUS(status)) {
                                ++unsuccessful;
                        } else {
                                ++successful;
                        }
                }
        }

        dprintf(1, "Successful: %d\nUnsuccessful: %d\n", successful, unsuccessful);
}

void execute_process(const char* cmd) {
        pid_t pid = fork();
        if(pid < 0) {
                err(2, "Could not fork");
        }

        if(pid == 0) {
                if(execlp(cmd, cmd, (char*)NULL) == -1) {
                        err(3, "Could not execute command %s", cmd);
                }
        }
}
