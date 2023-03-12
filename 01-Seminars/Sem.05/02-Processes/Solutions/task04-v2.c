s62226@astero:~/testDir/01$ vim main.c
#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(void) {
        int n = 100;

        pid_t pid = fork();

        if(pid < 0) {
                err(1, "Could not fork");
        }

        for(int i = 1; i <= n; ++i) {
                if(pid > 0) { //parent
                        printf("I am parent on iteration: %d\n", i);
                } else { //child
                        printf("I am child on iteration: %d\n", i);
                }
        }
}