#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#define FILE_COUNT 3

void close_all(void);

int fd[FILE_COUNT] = {-1, -1, -1};

struct Interval {
    uint32_t position;
    uint32_t length;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "Invalid argument count");
    }

    const char* f1 = argv[1];
    const char* f2 = argv[2];
    const char* f3 = argv[3];

    int success = 0; // Flag to track overall success
    do {
        // Open File 1
        if ((fd[0] = open(f1, O_RDONLY)) < 0) {
            warn("Could not open file %s", f1);
            break;
        }

        // Open File 2
        if ((fd[1] = open(f2, O_RDONLY)) < 0) {
            warn("Could not open file %s", f2);
            break;
        }

        // Open or create File 3
        if ((fd[2] = open(f3, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
            warn("Could not open file %s", f3);
            break;
        }

        // Validate file sizes
        struct stat st1, st2;
        if (stat(f1, &st1) < 0 || stat(f2, &st2) < 0) {
            warn("Error with stat");
            break;
        }
        if (st1.st_size == 0 || st2.st_size == 0) {
            warnx("File 1 or File 2 is empty");
            break;
        }

        // Check file size alignment
        if (st1.st_size % sizeof(struct Interval) != 0 || st2.st_size % sizeof(uint32_t) != 0) {
            warnx("Invalid file structure");
            break;
        }

        // Process intervals from File 1
        struct Interval interval;
        ssize_t bytesRead;
        while ((bytesRead = read(fd[0], &interval, sizeof(interval))) == sizeof(interval)) {
            if (interval.position >= st2.st_size || (interval.position + interval.length > st2.st_size)) {
                continue; // Skip invalid intervals
            }

            // Seek to the position in File 2
            if (lseek(fd[1], interval.position, SEEK_SET) < 0) {
                warn("Error seeking in File 2");
                break;
            }

            // Read and write numbers
            for (uint32_t i = 0; i < interval.length; ++i) {
                uint32_t number;
                if (read(fd[1], &number, sizeof(number)) != sizeof(number)) {
                    warn("Error reading from File 2");
                    break;
                }
                if (write(fd[2], &number, sizeof(number)) != sizeof(number)) {
                    warn("Error writing to File 3");
                    break;
                }
            }
        }

        if (bytesRead < 0) {
            warn("Error reading from File 1");
            break;
        }

        success = 1; // All operations succeeded
    } while (0);

    close_all();

    if (!success) {
        err(1, "Operation failed");
    }

    return 0;
}

void close_all(void) {
    int tempError = errno;
    for (int i = 0; i < FILE_COUNT; ++i) {
        if (fd[i] >= 0) {
            close(fd[i]);
        }
    }
    errno = tempError;
}
