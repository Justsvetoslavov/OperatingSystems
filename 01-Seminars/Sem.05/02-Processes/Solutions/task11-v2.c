#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
        if (argc < 2) {
                errx(1, "Invalid argument count");
        }

        int successful = 0;
        int failed = 0;
        for(int i = 1; i < argc; ++i) {
                pid_t wpid = fork();
                if(wpid < 0) {
                        err(2, "Could not fork");
                }
                if(0 == wpid) { // child
                        if(execlp(argv[i], argv[i], (char*)NULL) < 0) {
                                err(3, "Could not exec command %s", argv[i]);
                        }
                }

                int status;
                if(wait(&status) < 0) {
                        err(4, "Could not wait for child");
                }
                if(!WIFEXITED(status) || WEXITSTATUS(status)) {
                        ++failed;
                } else {
                        ++successful;
                }
        }

        printf("Successful: %d\nFailed: %d\n", successful, failed);
}
