#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>

int main(void) {
        pid_t pid = fork();
        if(pid < 0) {
                err(1, "Could not fork");
        }

        const int n = 100; // counter for loop
        int status;
        if(pid > 0) {
                wait(&status);
                for(int i = 0; i < n; i++) {
                        dprintf(1, "%d: Yes\n", i);
                }
        } else {
                for(int i = 0; i < n; i++) {
                        dprintf(1, "%d: No\n", i);
                }
        }

        return 0;
}
