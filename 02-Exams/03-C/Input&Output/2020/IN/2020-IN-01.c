#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

int fds[3];

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t _;
    uint32_t __;
} header;

typedef struct __attribute__((packed)) {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
} data_first;

typedef struct __attribute__((packed)) {
    uint32_t offset;
    uint16_t original_word;
    uint16_t new_word;
} data_second;

void close_all(void);
int open_read(const char* file_name);
int open_create(const char* file_name);
void write_safev1(int fd, uint8_t b, const char* file_name);
void lseek_safev1(int fd, uint16_t offset, int whence);
void write_safev2(int fd, uint16_t b, const char* file_name);
void lseek_safev2(int fd, uint32_t offset, int whence);

int open_read(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        err(2, "Could not open file %s", file_name);
    }

    return fd;
}

void close_all(void) {
    int errno_ = errno;
    for (int i = 0; i < 3; i++) {
        if (fds[i] >= 0) {
            close(fds[i]);
        }
    }

    errno = errno_;
}

int open_create(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
        err(2, "Could not open file %s", file_name);
    }

    return fd;
}

void write_safev1(int fd, uint8_t b, const char* file_name) {
    if (write(fd, &b, sizeof(b)) != sizeof(b)) {
        close_all();
        errx(6, "Could not write to file %s", file_name);
    }
}

void write_safev2(int fd, uint16_t b, const char* file_name) {
    if (write(fd, &b, sizeof(b)) != sizeof(b)) {
        close_all();
        errx(6, "Could not write to file %s", file_name);
    }
}

void lseek_safev1(int fd, uint16_t offset, int whence) {
    if (lseek(fd, offset, whence) == -1) {
        close_all();
        err(7, "Could not lseek");
    }
}

void lseek_safev2(int fd, uint32_t offset, int whence) {
    if (lseek(fd, offset, whence) == -1) {
        close_all();
        err(7, "Could not lseek");
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "Invalid argumetns. Usage: %s <patch> <f1> <f2>", argv[0]);
    }

    const char *patch = argv[1], *f1 = argv[2], *f2 = argv[3];
    fds[0] = open_read(patch);
    fds[1] = open_read(f1);
    fds[2] = open_create(f2);

    header h;
    if (read(fds[0], &h, sizeof(h)) == -1) {
        close_all();
        err(3, "Could not read from file %s", patch);
    }

    if (h.magic != 0xEFBEADDE || h.header_version != 0x001) {
        close_all();
        errx(4, "Invalid header");
    }

    int bytes_read;
    if (h.data_version == 0x00) {
        uint8_t original_byte;
        while((bytes_read = read(fds[1], &original_byte, sizeof(original_byte))) > 0) {
            write_safev1(fds[2], original_byte, f2);
        }

        if (bytes_read == -1) {
            close_all();
            err(5, "Could not read from file %s", f1);
        }

        data_first data;
        while ((bytes_read = read(fds[0], &data, sizeof(data))) > 0) {
            lseek_safev1(fds[2], data.offset, SEEK_SET);
            if (read(fds[2], &original_byte, sizeof(original_byte)) == -1) {
                close_all();
                err(5, "Could not read from file %s", f1);
            }

            if (original_byte != data.original_byte) {
                close_all();
                errx(8, "Invalid patch content");
            }

            lseek_safev1(fds[2], data.offset, SEEK_SET);
            write_safev1(fds[2], data.new_byte, f2);
        }
    }
    else if (h.data_version == 0x01){
        uint16_t original_word;
        while((bytes_read = read(fds[1], &original_word, sizeof(original_word))) > 0) {
            write_safev2(fds[2], original_word, f2);
        }

        if (bytes_read == -1) {
            close_all();
            err(5, "Could not read from file %s", f1);
        }

        data_second data;
        while ((bytes_read = read(fds[0], &data, sizeof(data))) > 0) {
            lseek_safev2(fds[2], data.offset, SEEK_SET);
            if (read(fds[2], &original_word, sizeof(original_word)) == -1) {
                close_all();
                err(5, "Could not read from file %s", f1);
            }

            if (original_word != data.original_word) {
                close_all();
                errx(8, "Invalid patch content");
            }

            lseek_safev2(fds[2], data.offset, SEEK_SET);
            write_safev2(fds[2], data.new_word, f2);
        }
    } else {
        errx(8, "Invalid data version")
    }

    close_all();
    exit(0);
}