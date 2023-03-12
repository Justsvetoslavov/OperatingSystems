// Solution 1:
#include <unistd.h>
#include <err.h>
#include <stddef.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
        if(argc != 3) {
                errx(1, "Error: Invalid number of arguments %d", argc - 1);
        }

        const char* first_cmd = argv[1];
        const char* second_cmd = argv[2];

        pid_t pid = fork();
        if(pid < 0) {
                err(2, "Could not fork");
        }

        int status;
        if(pid == 0) { // child
                if(execlp(first_cmd, first_cmd, (char *)NULL) == -1) {
                        err(3, "could not exec command %s", first_cmd);
                }
        } else { // Parent
                if(wait(&status) == -1) {
                        err(4," Could not wait child");
                }

                if(!WIFEXITED(status)) {
                        errx(42, "%s did not terminate normally", first_cmd);
                }

                if(WEXITSTATUS(status)) {
                        errx(42, "%s exit status is not 0", first_cmd);
                }

                if(execlp(second_cmd, second_cmd, (char *)NULL) == -1) {
                        err(2, "Could not exec command %s", second_cmd);
                }
        }

        return 0;
}
