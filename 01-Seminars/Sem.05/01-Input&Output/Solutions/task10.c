#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

const char* min = "--min";
const char* max = "--max";
const char* print = "--print";

void close_safe(int fd);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Error: Invalid number of arguments: %d", argc - 1);
	}

    const char* option = argv[1];
	if((strcmp(option, min) != 0) && (strcmp(option, max) != 0) && (strcmp(option, print) != 0)) {
		errx(2, "Error: Invalid option: %s", option);
	}

	const char* filename = argv[2];
	struct stat st;
	if(stat(filename, &st) == -1) {
		err(3, "Error while stat file %s", filename);
	}

    if (st.st_size == 0) {
        errx(4, "File %s is empty", filename);
    }

    if ((fd = open(filename, O_RDONLY)) == -1) {
        errx(5, "Error while opening file %s", filename);
    }

    unsigned char byte;
    int bytes_count = 0;
    if (strcmp(option, min) == 0) {
        unsigned char minByte;
        if (read(fd, &minByte, sizeof(minByte)) == -1) {
            close_safe(fd);
            err(5, "Error while reading from file %s", filename);
        }
        while ((bytes_count = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
            if (minByte > byte) {
                minByte = byte;
            }
        }
        dprintf(1, "%u\n", minByte);
    } else if (strcmp(option, max) == 0) {
        unsigned char maxByte = 0;
        if (read(fd, &maxByte, sizeof(maxByte)) == -1) {
            close_safe(fd);
            err(6, "Error while reading from file %s", filename);
        }
        while ((bytes_count = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
            if (maxByte < byte) {
                maxByte = byte;
            }
        }
        dprintf(1, "%u\n", maxByte);
    } else {
        while ((bytes_count = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
            dprintf(1, "%u\n", byte);
        }
    }

    if (bytes_count == -1) {
        close_safe(fd);
        err(7, "Error while reading from %s", filename);
    }

    close_safe(fd);
    
    return 0;
}

void close_safe(int fd) {
    int errno_ = errno;
    close(fd);
    errno = errno_;
}
