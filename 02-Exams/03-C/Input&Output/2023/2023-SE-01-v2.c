#include <fcntl.h>    // For open, O_RDONLY, O_WRONLY, O_CREAT, etc.
#include <unistd.h>   // For read, write, close
#include <err.h>      // For err and errx
#include <errno.h>    // For errno
#include <stdint.h>   // For fixed-width types like uint8_t
#include <stdio.h>    // For printf
#include <stdlib.h>   // For exit

// Global file descriptors for source and destination
int fd[] = {-1, -1};  // File descriptors for input and output files
uint8_t fileCount = 2; // Number of files to handle

// Function declaration for closing all file descriptors
void close_all(void);

// Function definition for closing all open file descriptors
void close_all(void) {
    for (uint8_t i = 0; i < fileCount; i++) {
        if (fd[i] != -1) {
            close(fd[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    // Validate the number of arguments
    if (argc != 3) {
        errx(1, "Invalid number of arguments: %d", argc - 1);
    }

    // Get source and destination file paths
    const char* source = argv[1];
    const char* dest = argv[2];

    // Open the source file for reading
    if ((fd[0] = open(source, O_RDONLY)) == -1) {
        err(2, "Could not open source file %s", source);
    }

    // Open (or create) the destination file for writing
    if ((fd[1] = open(dest, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1) {
        close(fd[0]); // Ensure the source file is closed before exiting
        err(3, "Could not open or create destination file %s", dest);
    }

    // Buffer to store message contents
    uint8_t contents[256];  // Message buffer with maximum size of 256 bytes
    uint8_t read_byte;      // Single byte read from the source file
    uint8_t index = 0;      // Current index for the message buffer
    uint8_t n_size = 0;     // Size of the current message (second byte)
    uint8_t checksum = 0;   // Computed checksum for the current message

    // Read the source file byte by byte
    ssize_t bytesRead;
    while ((bytesRead = read(fd[0], &read_byte, sizeof(read_byte))) == sizeof(read_byte)) {
        // If the byte is 0x55 (start byte) and we are not in the middle of a message
        if (read_byte == 0x55 && index == 0) {
            contents[index++] = read_byte;  // Store the start byte
            checksum = read_byte;          // Initialize checksum with the start byte
            continue;
        }

        // If we are reading the second byte (message size)
        if (index == 1) {
            n_size = read_byte;            // Set the message size
            contents[index++] = read_byte; // Store the size byte
            checksum ^= read_byte;         // Update the checksum

            // Validate the message size
            if (n_size <= 3) {
                // Invalid message: too short or exceeds buffer capacity
                index = 0; // Reset for the next message
                checksum = 0;
                continue; // Skip invalid messages
            }
            continue;
        }

        // If we are reading the data bytes (3rd byte onwards)
        if (index > 1 && index < n_size) {
            contents[index++] = read_byte; // Store the current byte in the buffer
            checksum ^= read_byte;         // Update the checksum

            // If we have read the full message
            if (index == n_size) {
                // Validate the checksum
                if (checksum == 0) { // Checksum should be 0 for valid messages
                    // Write the valid message to the destination file
                    if (write(fd[1], contents, n_size) != n_size) {
                        close_all(); // Ensure all files are closed before exiting
                        err(4, "Error while writing to destination file %s", dest);
                    }
                }
                // Reset for the next potential message
                index = 0;
                checksum = 0;
            }
        }
    }

    // Check if the last `read()` call failed
    if (bytesRead == -1) {
        close_all(); // Ensure all files are closed before exiting
        err(5, "Error while reading from source file %s", source);
    }

    // Close all open file descriptors
    close_all();
    return 0; // Indicate successful execution
}
