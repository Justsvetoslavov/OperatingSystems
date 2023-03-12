#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <err.h>
#include <string.h>

void read_fd(int, uint8_t*, bool); // Change to pointer to update line count

int main(int argc, char* argv[]) {
    bool printLines = false;
    uint8_t lines = 1; // Start line numbering from 1
    uint8_t ind_first_file = 1;

    if(argc > 1 && 0 == strcmp(argv[1], "-n")) {
        printLines = true;
        ind_first_file = 2;
    }

    if(argc == ind_first_file) {
        read_fd(0, &lines, printLines); // Always read from stdin if no files are specified
    } else {
        for(int i = ind_first_file; i < argc; ++i) {
            if(0 == strcmp(argv[i], "-")) {
                read_fd(0, &lines, printLines);
            } else {
                int fd = open(argv[i], O_RDONLY);
                if(fd == -1) {
                    warnx("Could not open file %s", argv[i]);
                    continue;
                }
                read_fd(fd, &lines, printLines);
                close(fd); // Close the file after reading
            }
        }
    }

    return 0;
}

void read_fd(int fd, uint8_t* line, bool printLines) {
    int read_bytes;
    uint8_t s;
    bool at_start = true;

    while((read_bytes = read(fd, &s, sizeof(s))) > 0) {
        if(printLines && at_start) {
            dprintf(1, "%d: ", *line);
            at_start = false;
        }
        if(write(1, &s, sizeof(s)) != sizeof(s)) {
            close(fd);
            err(2, "Error writing");
        }
        if('\n' == s) {
        	(*line)++; // Increment line
        	at_start = true;
		}
    }
    if(read_bytes < 0) {
        close(fd);
        err(3, "Error reading");
    }
}
