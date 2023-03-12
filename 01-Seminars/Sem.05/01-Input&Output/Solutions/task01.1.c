#include <fcntl.h> // open funciton
#include <err.h> // err & errx functions
#include <unistd.h> // close function

int main(int argc, char* argv[]) {
        if(argc != 3) {
                errx(1, "Invalid argument count");
        }

        const char* file1=argv[1];
        const char* file2=argv[2];

        int fd1=open(file1, O_RDONLY);
        if(fd1 < 0) {
                err(2, "Could not open file %s", file1);
        }
        int fd2=open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd2 < 0) {
                close(fd1);
                err(3, "Could not open or create file %s", file2);
        }

        char s;
        int read_bytes;
        while(sizeof(s) == (read_bytes = read(fd1, &s, sizeof(s)))) {
                if(sizeof(s) != write(fd2, &s, sizeof(s))) {
                        close(fd1);
                        close(fd2);
                        err(4, "Error writing to file %s", file2);
                }
        }

        if(read_bytes < 0) {
                close(fd1);
                close(fd2);
                err(4, "Error reading from file %s", file1);
        }

        close(fd1);
        close(fd2);
}
