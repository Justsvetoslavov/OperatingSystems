#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
        if(2 != argc) {
                errx(1, "Invalid argument count");
        }

        const char* filename=argv[1];
        int fd=open(filename, O_RDONLY);
        if(fd < 0) {
                err(2, "Could not open file %s", filename);
        }

        char s;
        int read_bytes;
        int newLines=0;
        while(sizeof(s) == (read_bytes = read(fd, &s, sizeof(s)))) {
                if(sizeof(s) != write(1, &s, sizeof(s))) {
                        close(fd);
                        err(3, "Could not write to stdout");
                }
                if('\n' == s) {
                        ++newLines;
                        if(10 == newLines) {
                                break;
                        }
                }
        }

        if(read_bytes < 0) {
                close(fd);
                err(4, "Could not read from file %s", filename);
        }

        close(fd);
}
