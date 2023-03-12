#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if(argc != 4) {
        errx(1, "Error: Invalid number of arguments - %d", argc - 1);
    }
    
    int status;
    for(int i = 1; i < argc; ++i) {
        pid_t pid = fork();

        if(pid < 0) {
            err(2, "Could not fork");
        }
        if(pid == 0) { // Child process
            if(execlp(argv[i], argv[i], (char*)NULL) == -1) {
                err(3, "could not execlp");
            }
        } else { // Parent process
            pid_t wpid = wait(&status);
            if(wpid == -1) {
                err(4, "Could not wait for child");
            }
            if(!(WIFEXITED(status))) {
                err(6, "Child did not terminate normally");
            }
            dprintf(1, "Process: %d\nExit status: %d\n\n", wpid, WEXITSTATUS(status));
        }
    }
}