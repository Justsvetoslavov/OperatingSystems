#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void close_all(void);
int open_read(const char*);
int open_creat(const char*);
uint16_t manchester_encode(uint8_t);
void write_safe(int, uint16_t, const char*);

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "Invalid number of arguments: %d", argc - 1);
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    fds[0] = open_read(input_file);
    fds[1] = open_creat(output_file);

    struct stat st;
	if(stat(input_file, &st) == -1) {
		err(3, "Error while stat file %s", input_file);
	}

    if (st.st_size == 0) {
        errx(3, "File %s is empty", input_file);
    } else if(st.st_size % sizeof(uint8_t) != 0) { // Optional
        errx(3, "File %s is not compatible in terms of size", input_file);
    }

    uint8_t num;
    int bytes_read;
    while((bytes_read = read(fds[0], &num, sizeof(num))) > 0) {
        uint16_t result = manchester_encode(num);
        write_safe(fds[1], result, output_file);
    }

    if (bytes_read == -1) {
        close_all();
        err(3, "Could not read from file %s", input_file);
    }

    close_all();

    return 0;
}

uint16_t manchester_encode(uint8_t num) {
    uint16_t result = 0;
    for (int i = 7; i >= 0; i--) {
        if (num >> i & 1) {
            result ^= (1 << (2*i +1));
        } else {
            result ^= (1 << (2*i));
        }
    }

    return result;
}


void write_safe(int fd, uint16_t num, const char* file_name) {
    int bytes_written;
    if (sizeof(num) != write(fd, &num, sizeof(num))) {
        close_all();
        errx4, "Could not write to file %s", file_name);
    }
}

int open_read(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        err(2, "Could not open file %s", file_name);
    }

    return fd;
}

int open_creat(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) {
        err(2, "Could not open file %s", file_name);
    }
    return fd;
}

void close_all(void) {
    int tempError = errno;
    for (int i = 0; i < fileCount; ++i) {
        if (fds[i] >= 0) {
            close(fds[i]);
        }
    }
    errno = tempError;
}