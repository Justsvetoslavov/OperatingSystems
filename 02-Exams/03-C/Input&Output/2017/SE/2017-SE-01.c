#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>

typedef struct __attribute__((packed)) {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
} triplet;

int open_read(const char* file_name);
int open_create(const char* file_name);
void write_safe(int fd, const triplet t);
void close_all(void);

const uint8_t fileCount = 3;
int fds[] = {-1 , -1, -1};

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "Invalid arguments. Usage: %s <f1> <f2> <patch>", argv[0]);
    }

    const char* f1 = argv[1], *f2 = argv[2], *patch = argv[3];
    struct stat s1, s2;
    if (stat(f1, &s1) == -1 || stat(f2, &s2) == -1) {
        err(3, "Could not stat");
    }

    if (s1.st_size != s2.st_size) {
        errx(4, "Inconsistent file content");
    }

    fds[0] = open_read(f1);
    fds[1] = open_read(f2);
    fds[2] = open_create(patch);

    triplet t;

    uint8_t original_byte, new_byte;
    int bytes_count1, bytes_count2;
    uint16_t offset = 0;

    while (((bytes_count1 = read(fds[0], &original_byte, sizeof(original_byte))) == sizeof(original_byte)) &&
           (bytes_count2 = read(fds[1], &new_byte, sizeof(new_byte)) == sizeof(new_byte))) {
        if (original_byte != new_byte) {
            t.original_byte = original_byte;
            t.new_byte = new_byte;
            t.offset = offset;
            write_safe(fds[2], t);
        }
        ++offset;
    }

    if (bytes_count1 == -1 || bytes_count2 == -1) {
        close_all();
        err(9, "Could not read from file");
    }

    close_all();
    return 0;
}

void write_safe(int fd, const triplet t) {
    if (write(fd, &t, sizeof(t)) != sizeof(t)) {
        close_all();
        errx(6, "Could not write to file");
    }
}

int open_read(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        close_all();
        err(2, "Could not open file %s", file_name);
    }

    return fd;
}

int open_create(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
        close_all();
        err(2, "Could not open file %s", file_name);
    }

    return fd;
}

void close_all(void) {
    int errno_ = errno;
    for (int i = 0; i < fileCount; i++) {
        if (fds[i] >= 0) {
            close(fds[i]);
        }
    }

    errno = errno_;
}