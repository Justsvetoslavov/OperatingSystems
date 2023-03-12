#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
        if(argc != 2) {
                errx(1, "Error: Invalid number of arguments");
        }

        pid_t pid = fork();
        if(pid < 0) {
                err(2, "Could not fork");
        }

        const char* cmd = argv[1];
        if(pid == 0) {
                if(execlp(cmd, cmd, (char*)NULL) == -1) {
                        err(3, "Error execling command %s", cmd);
                }
        }

        int status;
        if(wait(&status) == -1) {
                err(4, "Could not wait for child");
        }

        if(!(WIFEXITED(status))) {
                errx(5, "Child did not terminate normally");
        }

        if(WEXITSTATUS(status)) {
                errx(6, "Child exit status is not 0");
        }

        printf("Executed command %s successfully\n", cmd);
}
