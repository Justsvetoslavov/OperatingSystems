#include <fcntl.h>    // For open, O_RDONLY, O_CREAT, etc.
#include <unistd.h>   // For read, write, lseek, close
#include <stdint.h>   // For uint8_t
#include <err.h>      // For err and errx

#define FIRST 0x55    // The first byte indicating the start of a message

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        errx(1, "Invalid argument count");
    }

    // Source and destination file paths
    const char* sourceFilename = argv[1];
    const char* destFilename = argv[2];

    // Open the source file for reading
    int fd1 = open(sourceFilename, O_RDONLY);
    if (fd1 < 0) {
        err(2, "Could not open file %s", sourceFilename);
    }

    // Open (or create) the destination file for writing
    int fd2 = open(destFilename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd2 < 0) {
        close(fd1); // Ensure the source file is closed before exiting
        err(3, "Could not open or create file %s", destFilename);
    }

    uint8_t byte;      // Variable to store a single byte read from the file
    int read_bytes;    // Variable to store the result of the read operation

    // Read through the source file byte by byte
    while (sizeof(uint8_t) == (read_bytes = read(fd1, &byte, sizeof(uint8_t)))) {
        // Check if the byte matches the "start of message" indicator
        if (FIRST == byte) {
            uint8_t checksum = byte;  // Initialize checksum with the first byte
            off_t pos = lseek(fd1, 0, SEEK_CUR);  // Save the current position in the file
            if (pos < 0) {
                close(fd1);
                close(fd2);
                err(4, "Could not lseek file %s", sourceFilename);
            }

            // Read the second byte, which represents the length of the message (N)
            uint8_t second;
            if (read(fd1, &second, sizeof(uint8_t)) != sizeof(uint8_t)) {
                close(fd1);
                close(fd2);
                err(5, "Could not read from file %s", sourceFilename);
            }

            // Skip invalid messages with length <= 3
            if (second <= 3) {
                warnx("Second byte was invalid, skipping message");
                continue;
            }

            checksum ^= second;  // Update the checksum with the second byte

            // Process the data bytes (bytes 3 to N-1)
            for (uint8_t i = 3; i < second; ++i) {
                uint8_t number;
                if (read(fd1, &number, sizeof(uint8_t)) != sizeof(uint8_t)) {
                    close(fd1);
                    close(fd2);
                    err(5, "Could not read from file %s", sourceFilename);
                }
                checksum ^= number;  // Update the checksum with each data byte
            }

            // Read the last byte (checksum byte)
            uint8_t last;
            if (read(fd1, &last, sizeof(uint8_t)) != sizeof(uint8_t)) {
                close(fd1);
                close(fd2);
                err(5, "Could not read from file %s", sourceFilename);
            }

            // Verify the checksum
            if (last != checksum) {
                // If the checksum does not match, skip the message
                continue;
            }

            // Reset the file position to the start of the valid message
            if (lseek(fd1, pos, SEEK_SET) < 0) {
                close(fd1);
                close(fd2);
                err(4, "Could not lseek file %s", sourceFilename);
            }

            // Write the valid message to the output file
            for (uint8_t i = 1; i <= second; ++i) {
                uint8_t num;
                if (read(fd1, &num, sizeof(uint8_t)) != sizeof(uint8_t)) {
                    close(fd1);
                    close(fd2);
                    err(5, "Could not read from file %s", sourceFilename);
                }

                if (write(fd2, &num, sizeof(uint8_t)) != sizeof(uint8_t)) {
                    close(fd1);
                    close(fd2);
                    err(6, "Could not write to file %s", destFilename);
                }
            }
        }
    }

    // Check if there was an error during the read loop
    if (read_bytes < 0) {
        close(fd1);
        close(fd2);
        err(4, "Could not read from file %s", sourceFilename);
    }

    // Close the files before exiting
    close(fd1);
    close(fd2);
    return 0;  // Indicate successful execution
}
