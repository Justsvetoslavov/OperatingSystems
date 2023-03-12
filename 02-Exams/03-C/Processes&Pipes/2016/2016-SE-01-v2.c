
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        errx(1, "Invalid argument count");
    }

    const char* filename = argv[1];
    struct stat st;

    if(stat(filename, &st) < 0) {
        err(2, "Could not stat file. Maybe it doesn't exist?");
    }

    if(st.st_size == 0) {
        errx(3, "File is empty?");
    }

    int pfd[2];
    if(pipe(pfd) < 0) {
        err(4, "Could not pipe.");
    }

    pid_t wpid = fork();
    if(wpid < 0) {
        err(5, "Could not fork");
    }

    if(0 == wpid) {
        close(pfd[0]);
        if(dup2(pfd[1], 1) < 0) {
            err(6, "Could not dup in child process");
        }
        if(execlp("cat", "cat", filename, (char*)NULL) < 0) {
            err(7, "Could not execlp with command 'cat' for file: %s", filename);
        }
    }

    close(pfd[1]);
    int status;
    if(wait(&status) < 0) {
        err(8, "Could not wait for child process");
    }
    if(!WIFEXITED(status) || WEXITSTATUS(status)) {
        err(9, "Child process failed or did not terminate normally");
    }

    if(dup2(pfd[0], 0) < 0) {
        err(10, "Could not dup in parent process");
    }
    if(execlp("sort", "sort", filename, (char*)NULL) < 0) {
        err(11, "Could not execlp command sort for file %s", filename);
    }
}
