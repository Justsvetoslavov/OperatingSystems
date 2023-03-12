#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <stdint.h>

#define MAX_FILE_SIZE 524288

int main(int argc, char* argv[]) {
    if (argc != 3) {
        err(1, "Invalid argument count");
    }

    const char* source = argv[1];
    const char* dest = argv[2];

    struct stat st;
    if (stat(source, &st) < 0) {
        err(2, "Could not stat file %s", source);
    }
    if (st.st_size == 0 || (st.st_size % sizeof(uint16_t) != 0) || (st.st_size / sizeof(uint16_t) > MAX_FILE_SIZE)) {
        errx(3, "Invalid file size/count");
    }

    int fd1 = open(source, O_RDONLY);
    if (fd1 == -1) {
        err(4, "Failed to open source file %s", source);  // Handle error opening source file
    }

    int fd2 = open(dest, O_CREAT | O_TRUNC | O_WRONLY, 0644); // Set permissions for new file
    if (fd2 == -1) {
        err(5, "Failed to open destination file %s", dest);  // Handle error opening destination file
    }

    dprintf(fd2, "uint16_t arrN[] = { ");
    uint16_t num;
    uint32_t count = 0;
    ssize_t bytesRead;
    
    // Loop to read and write the numbers to the output file
    while ((bytesRead = read(fd1, &num, sizeof(num))) > 0) {
        ++count;
        if (count > 1) {
            dprintf(fd2, ", ");  // Add a comma between numbers, but not after the last one
        }
        dprintf(fd2, "%u", num);
    }

    if (bytesRead == -1) {
        err(6, "Error reading from source file");
    }

    dprintf(fd2, "};\nuint32_t size = %u;", count);

    // Close the file descriptors
    close(fd1);
    close(fd2);

    return 0;
}
