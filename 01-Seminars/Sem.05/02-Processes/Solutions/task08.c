#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void safe_close(int);

int main(int argc, char* argv[]) {
        if(argc != 2) {
                errx(1, "Invalid number of arguments: %d", argc - 1);
        }

        const char* filename = argv[1];

        int fd;
        if((fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
                err(2, "Could not open file %s", filename);
        }

        pid_t pid = fork();
        if(pid < 0) {
                safe_close(fd);
                err(2, "Could not fork");
        }

        int status;
        if(pid == 0) { // Child process
                const char strout[] = "foobar";
                if(write(fd, strout, strlen(strout)) != (ssize_t)strlen(strout)) {
                        safe_close(fd);
                        err(3, "Could not write to file %s", filename);
                }
        } else { // Parent process
                if(wait(&status) == -1) {
                        safe_close(fd);
                        err(4, "Could not wait for child process");
                }

                if(!(WIFEXITED(status))) {
                        safe_close(fd);
                        err(5, "Child did not terminate normally");
                }

                if(WEXITSTATUS(status)) {
                        safe_close(fd);
                        err(6, "Child exit status is not 0");
                }

                if(lseek(fd, 0, SEEK_SET) == -1) {
                        safe_close(fd);
                        err(7, "Could not lseek file %s", filename);
                }

                char symbol;
                int bytes_read;
                while((bytes_read = read(fd, &symbol, sizeof(char))) == sizeof(char)) {
                        dprintf(1, "%c ", symbol);
                }
                if(bytes_read < 0) {
                        safe_close(fd);
                        err(8, "Could not read from file %s", filename);
                }

                printf("\n");
        }

        safe_close(fd);

        return 0;
}

void safe_close(int fd) {
        int tempError = errno;
        if(fd >= 0) {
                close(fd);
        }
        errno = tempError;
}
