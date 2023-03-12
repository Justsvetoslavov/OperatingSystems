#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
        int n = 100;

        pid_t pid = fork();

        if(pid < 0) {
                err(1, "Could not fork");
        }
        for(int i = 1; i <= n; ++i) {
                if(pid > 0) { //parent
                        if(1 == i ) {
                                int status;
                                pid_t child_pid = wait(&status);
                                if(child_pid < 0) {
                                        err(2, "Could not wait for child");
                                }
                        }
                        printf("I am parent on iteration: %d\n", i);
                } else { //child
                        printf("I am child on iteration: %d\n", i);
                }
        }
}
