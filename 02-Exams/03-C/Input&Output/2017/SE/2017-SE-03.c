#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>

typedef struct __attribute__((packed))  {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
} triplet_t;

const uint8_t fileCount = 3;
int fds[] = {-1, -1, -1};

int safe_open(const char*);
int safe_create(const char*);
void close_all(void);

int main(int argc, char* argv[]) {
    if(argc != 4) {
        errx(1, "Error: Invalid number of arguments: %d", argc - 1);
    }

    const char* patch = argv[1];
    const char* f1 = argv[2];
    const char* f2 = argv[3];

    struct stat s1, s2;
    if(stat(patch, &s1) == -1) {
        err(2, "Could not stat file %s", patch);
    }

    if(stat(f1, &s2) == -1) {
        err(3, "Cou;d not stat file %s", f1);
    }

    uint32_t size1 = s1.st_size;
    uint32_t size2 = s2.st_size;

    if(0 == size1 || 0 == size2) {
        err(4, "File %s or %s is empty", patch, f1);
    }

    fds[0] = safe_open(patch);
    fds[1] = safe_open(f1);
    fds[2] = safe_create(f2);

    triplet_t t;
    if(read(fds[0], &t, sizeof(t)) != sizeof(t)) {
        err(7, "Could not read from file %s", patch);
    }

    uint8_t s;

    for(uint32_t i = 0; i < size2; ++i) {
        if(read(fds[1], &s, sizeof(s)) != sizeof(s)) {
            close_all();
            err(7, "Could not read from file %s", f1);
        }

        if(i == t.offset) {
            if(s != t.original_byte) {
                close_all();
                errx(8, "Invalid original byte");
            }
            s = t.new_byte;

            int read_bytes;
            if((read_bytes = read(fds[0], &t, sizeof(t))) != sizeof(t)) {
                if(read_bytes != 0) { // not EOF
                    err(8, "Could not read from file %s", patch);
                }
            }

            if(write(fds[2], &s, sizeof(s)) != sizeof(s)) {
                close_all();
                err(9, "Could not write to file %s", f2);
            }
        }
    }

    close_all();
    return 0;
}

void close_all(void) {
    int tempError = errno;
    for(int i = 0; i < fileCount; ++i) {
        if(fds[i] >= 0) {
            close(fds[i]);
        }
    }
    errno = tempError;
}

int safe_open(const char* filename) {
    int fd;
    if((fd = open(filename, O_RDONLY)) == -1) {
        close_all();
        err(2, "Could not open file %s", filename);
    }
    return fd;
}

int safe_create(const char* filename) {
    int fd;
    if((fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
        close_all();
        err(3, "Could not open/create file %s", filename);
    }

    return fd;
}
