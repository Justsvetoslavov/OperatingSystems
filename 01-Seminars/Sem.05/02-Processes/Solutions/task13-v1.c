#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/wait.h>
#include <err.h>

void execute_command(const char*);

int main(int argc, char* argv[]) {
        if(argc != 3) {
                errx(1, "Error: Invalid number of arguments: %d", argc - 1);
        }

        execute_command(argv[1]);
        execute_command( argv[2]);

        int status1, status2;
        const pid_t first = wait(&status1);
        if(-1 == first) {
                err(4, "Could not wait for any process");
        }

        if(WIFEXITED(status1) && WEXITSTATUS(status1)) {
                dprintf(1, "%d\n", first);
        } else {
                const pid_t second = wait(&status2);
                if(-1 == second) {
                        err(5, "Could not wait for second process");
                }
                if(WIFEXITED(status2) && !WEXITSTATUS(status2)) {
                        dprintf(1, "%d\n", second);
                } else {
                        dprintf(1, "%d\n", -1);
                }
        }
}

void execute_command(const char* cmd) {
        const pid_t pid = fork();

        if(-1 == pid) {
                err(2, "Could not fork");
        }

        if(0 == pid) {
                if(execlp(cmd, cmd, (char *)NULL) == -1) {
                        err(3, "Could not execlp %s", cmd);
                }
        }
}
