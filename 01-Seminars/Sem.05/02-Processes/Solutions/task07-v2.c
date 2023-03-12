#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
        if (argc != 4) {
                errx(1, "Invalid argument count");
        }

        for(int i = 1; i < argc; ++i) {
                pid_t pid = fork();
                if(pid < 0) {
                        err(2, "Could not fork parent process");
                }
                if(0 == pid) { //child
                        if(execlp(argv[i], argv[i], (char*)NULL) < 0) {
                                err(6, "Could not execute command: %s", argv[i]);
                        }
                }

                int status;
                pid_t child_pid = wait(&status);
                if(child_pid < 0) {
                        err(3, "Could not wait for child");
                }
                if(!WIFEXITED(status)) {
                        err(4, "Child did not terminate normally");
                }
                if(WEXITSTATUS(status)) { // Optional
                        warnx("Child exit status is not 0");
                }
                printf("Child proccess: %ld\nExit status: %d\n", (long)child_pid, status);
        }
}
