#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    uint16_t magic;
    uint16_t filetype;
    uint32_t count;
} header;

typedef struct {
    uint8_t dataOffset;  // Offset in data.bin
    uint8_t outOffset;   // Offset in out.bin
} ListEntry;

void close_all(void);
void read_safe(int fd, void* toWhere, ssize_t howMany, const char* file_name);
void write_safe(int fd, void* what, ssize_t howMany, const char* file_name);
void lseek_safe(int fd, size_t pos);

int fds[] = {-1, -1, -1};

void read_safe(int fd, void* toWhere, ssize_t howMany, const char* file_name) {
    int bytes_read = read(fd, toWhere, howMany);
    if (bytes_read < 0) {
        close_all();
        err(1, "Couldn't read from file %s", file_name);
    }
    if (bytes_read != howMany) {
        close_all();
        errx(2, "Couldn't read all data from file %s.", file_name);
    }
}

void write_safe(int fd, void* what, ssize_t howMany, const char* file_name) {
    int bytes_write = write(fd, what, howMany);
    if (bytes_write < 0) {
        close_all();
        err(3, "Couldn't write to file %s", file_name);
    }
    if (bytes_write != howMany) {
        close_all();
        errx(4, "Couldn't write all data to file %s.", file_name);
    }
}

void close_all() {
    int old_errno = errno;
    for (size_t i = 0; i < 3; i++) {
        if (fds[i] != -1) {
            close(fds[i]);
        }
    }
    errno = old_errno;
}

void lseek_safe(int fd, size_t pos) {
    if (lseek(fd, pos, SEEK_SET) < 0) {
        close_all();
        err(5, "Couldn't lseek");
    }
}

int open_safe(const char* file, int oflag) {
    int fd = open(file, oflag);
    if (fd == -1) {
        close_all();
        errx(6, "Couldn't open file %s", file);
    }
    return fd;
}

int open_safe_create(const char* file) {
    int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        close_all();
        errx(6, "Couldn't open file %s", file);
    }
    return fd;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(10, "Invalid argument count! Usage: ./main list.bin data.bin out.bin");
    }

    fds[0] = open_safe(argv[1], O_RDONLY);  // list.bin
    fds[1] = open_safe(argv[2], O_RDONLY);  // data.bin
    fds[2] = open_safe_create(argv[3]);     // out.bin

    header hList, hData, hOut;
    read_safe(fds[0], &hList, sizeof(hList), argv[1]);
    read_safe(fds[1], &hData, sizeof(hData), argv[2]);

    size_t headerLen = sizeof(header);
    const uint16_t MAGIC = 0x5A4D;

    // Validate Hoge file headers
    if (hList.magic != MAGIC || hData.magic != MAGIC) {
        close_all();
        errx(11, "Files not in Hoge system format.");
    }

    if (hList.filetype != 1 || hData.filetype != 2) {
        close_all();
        errx(12, "Invalid file types.");
    }

    // Read max offset in out.bin (to determine its required size)
    uint8_t maxOutOffset = 0;
    ListEntry entry;
    for (size_t i = 0; i < hList.count; i++) {
        read_safe(fds[0], &entry, sizeof(entry), argv[1]);
        if (entry.outOffset > maxOutOffset) {
            maxOutOffset = entry.outOffset;
        }
    }

    // Initialize out.bin header
    hOut.count = maxOutOffset + 1;
    hOut.magic = MAGIC;
    hOut.filetype = 3;
    write_safe(fds[2], &hOut, sizeof(hOut), argv[3]);

    // Write empty (zeroed) elements to out.bin
    uint64_t zero = 0;
    for (size_t i = 0; i < hOut.count; i++) {
        write_safe(fds[2], &zero, sizeof(zero), argv[3]);
    }

    // Process list.bin to move data from data.bin → out.bin
    lseek_safe(fds[0], headerLen); // Reset list.bin pointer
    for (size_t i = 0; i < hList.count; i++) {
        read_safe(fds[0], &entry, sizeof(entry), argv[1]);

        // Read data from data.bin at offset
        uint32_t dataValue;
        lseek_safe(fds[1], headerLen + entry.dataOffset * sizeof(uint32_t));
        read_safe(fds[1], &dataValue, sizeof(dataValue), argv[2]);

        // Convert to uint64_t and write to out.bin at offset
        uint64_t toWrite = dataValue;
        lseek_safe(fds[2], headerLen + entry.outOffset * sizeof(uint64_t));
        write_safe(fds[2], &toWrite, sizeof(toWrite), argv[3]);
    }

    close_all();
    return 0;
}
