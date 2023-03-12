#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void execute_command(const char* cmd);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "Error: Invalid number of arguments: %d", argc - 1);
    }

    const char* cmd1 = argv[1];
    const char* cmd2 = argv[2];
    const char* file_name = argv[3];

    int fd;
    if ((fd = open(file_name, O_RDWR | O_CREAT, S_IRWXU)) == -1) {
        err(2, "Error while opening file %s", file_name);
    }

    execute_command(cmd1);

    int status;
    if (wait(&status) == -1) {
        err(5, "Could not wait child process");
    }

    if(WIFEXITED(status)) {
        if (0 == WEXITSTATUS(status)) {
            //Option 1:
            if (write(fd, cmd1, strlen(cmd1)) == -1) {
                close(fd);
                err(6, "could not write to file %s", file_name);
            }
            write(fd, "\n", 1);

            //Option 2:
            //int len = strlen(argv[i]) + 1; // Including '\n'
            //if (len != dprintf(fd, "%s\n", argv[i])) {
            //    close(fd);
            //    err(6, "Could not write full content to file: %s", filename);
            //}
        } else {
            warnx("%s exit status not 0", cmd1);
        }
    } else {
        warnx("%s did not terminate normally", cmd1);
    }

    execute_command(cmd2);

    int status2;
    if (wait(&status2) == -1) {
        err(7, "Could not wait child process");
    }
    exit(0);
}

void execute_command(const char* cmd) {
    const pid_t pid = fork();
    if (-1 == pid) {
        err(3, "Err while fork process");
    }

    if (0 == pid) {
        if (execlp(cmd, cmd, (char *)NULL) == -1) {
            err(4, "Err while execlp cmd %s", cmd);
        }
    }
}