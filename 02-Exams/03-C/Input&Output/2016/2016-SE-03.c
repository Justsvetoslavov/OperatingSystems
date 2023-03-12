#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/param.h>

// Array to hold file descriptors for temporary files and the source file
int fds[3];

// Function declarations
void close_all(void);
int compareUint32_t(const void* a, const void* b);
void sortFile(const char* file_name);
void split(int halfCount, int fd, const char* temp_file, const char* source_file);
int openReadSafe(const char* file_name);
uint32_t readNumber(int fd, const char* file_name);
void merge(const char* temp1, const char* temp2, int temp1Size, int temp2Size, const char* source_file);
void writeSafe(int fd, uint32_t number, const char* file_name);

// Function to close all open file descriptors safely
void close_all(void) {
    int errno_ = errno;
    // Close all file descriptors in fds array
    for (int i = 0; i < 3; i++) {
        if (fds[i] >= 0) {
            close(fds[i]);
        }
    }
    // Restore errno
    errno = errno_;
}

// Comparison function to be used with qsort for sorting uint32_t numbers
int compareUint32_t(const void* a, const void* b) {
    const uint32_t* num1 = (const uint32_t*)a;
    const uint32_t* num2 = (const uint32_t*)b;

    if (*num1 < *num2) {
        return -1;  // num1 is less than num2
    } else if (*num1 > *num2) {
        return 1;   // num1 is greater than num2
    } else {
        return 0;   // num1 is equal to num2
    }
}

// Opens a file in read-only mode and checks for errors
int openReadSafe(const char* file_name) {
    int fd;
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        close_all();  // Close any open files before error exit
        err(10, "Error while opening file %s", file_name);  // Report error and exit
    }
    return fd;
}

// Reads a uint32_t number from a file descriptor
uint32_t readNumber(int fd, const char* file_name) {
    uint32_t number;
    // Attempt to read a uint32_t number from the file
    if (read(fd, &number, sizeof(number)) == -1) {
        close_all();  // Close open files on error
        err(11, "Error while reading from file %s", file_name);  // Report error and exit
    }
    return number;
}

// Writes a uint32_t number to a file descriptor
void writeSafe(int fd, uint32_t number, const char* file_name) {
    // Attempt to write the number to the file
    if (write(fd, &number, sizeof(uint32_t)) == -1) {
        close_all();  // Close open files on error
        err(10, "Error while writing to file %s", file_name);  // Report error and exit
    }
}

// Merges two sorted temporary files into the main file
void merge(const char* temp1, const char* temp2, int temp1Size, int temp2Size, const char* source_file) {
    // Open the two temporary files for reading
    fds[1] = openReadSafe(temp1);
    fds[2] = openReadSafe(temp2);

    // Open the source file for writing, truncating it to empty before writing
    if ((fds[0] = open(source_file, O_RDWR | O_TRUNC)) == -1) {
        err(11, "Error while opening file %s", source_file);
    }

    uint32_t num1 = readNumber(fds[1], temp1);  // Read the first number from temp1
    uint32_t num2 = readNumber(fds[2], temp2);  // Read the first number from temp2
    int counter = 0, minSize = MIN(temp1Size, temp2Size);  // Determine the smaller size for merging

    // Merge the two sorted chunks into the main file
    while (counter < minSize) {
        if (num1 < num2) {
            writeSafe(fds[0], num1, source_file);  // Write the smaller number (num1)
            num1 = readNumber(fds[1], temp1);  // Read the next number from temp1
        } else {
            writeSafe(fds[0], num2, source_file);  // Write the smaller number (num2)
            num2 = readNumber(fds[2], temp2);  // Read the next number from temp2
        }
        counter++;
    }

    // Write any remaining numbers from temp1 if temp2 is exhausted
    while (counter < temp1Size) {
        writeSafe(fds[0], num1, source_file);
        num1 = readNumber(fds[1], temp1);
        counter++;
    }

    // Write any remaining numbers from temp2 if temp1 is exhausted
    while (counter < temp2Size) {
        writeSafe(fds[0], num2, source_file);
        num2 = readNumber(fds[2], temp2);
        counter++;
    }
}

// Splits the data into smaller chunks, sorts them, and writes to a temporary file
void split(int halfCount, int fd, const char* temp_file, const char* source_file) {
    uint32_t number;
    uint32_t* half = malloc(halfCount * sizeof(uint32_t));  // Allocate memory for the chunk
    if (half == NULL) {
        close(fd);  // Close file descriptor in case of failure
        errx(7, "Error while memory allocation");  // Exit on memory allocation failure
    }

    int index = 0, bytes_count;
    // Read the numbers from the source file and store them in 'half' array
    while (index < halfCount && (bytes_count = read(fd, &number, sizeof(uint32_t))) == sizeof(uint32_t)) {
        half[index] = number;
        index++;
    }

    // If reading failed, clean up and exit
    if (bytes_count == -1) {
        close(fd);
        free(half);
        errx(8, "Error while reading from file %s", source_file);
    }

    // Sort the numbers in 'half' array using qsort
    qsort(half, halfCount, sizeof(uint32_t), compareUint32_t);

    int fd1;
    // Open a temporary file to store the sorted chunk
    if ((fd1 = open(temp_file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        free(half);  // Free memory on failure
        close(fd);   // Close file descriptor
        err(9, "Error while opening file %s", source_file);  // Exit on file opening failure
    }

    // Write the sorted numbers to the temporary file
    for (int i = 0; i < halfCount; i++) {
        writeSafe(fd1, half[i], temp_file);
    }

    close(fd1);  // Close the temporary file
    free(half);  // Free allocated memory
}

// Main function to perform the sorting process
void sortFile(const char* file_name) {
    struct stat st;
    // Get file status (size, etc.)
    if (stat(file_name, &st) == -1) {
        err(2, "Error stat file %s", file_name);
    }

    // Ensure the file size is a multiple of sizeof(uint32_t)
    if ((st.st_size % sizeof(uint32_t)) != 0) {
        errx(3, "File %s does not contain only uint32_t numbers", file_name);
    }

    ssize_t fileSize = st.st_size;
    // Ensure the number of elements does not exceed 100 million
    if ((fileSize / sizeof(uint32_t)) > 100000000) {
        errx(4, "File %s contains too many numbers", file_name);
    }

    int fd = openReadSafe(file_name);  // Open the source file
    char temp1[11] = "tempXXXXXX", temp2[11] = "tempXXXXXX";  // Temporary file names
    if (mkstemp(temp1) == -1 || mkstemp(temp2) == -1) {
        err(5, "Could not mkstemp");  // Exit if temp file creation fails
    }

    int numbersCount = fileSize / sizeof(uint32_t);  // Calculate the number of numbers
    int halfCount = numbersCount / 2;  // Split in half
    int otherHalfCount = numbersCount - halfCount;  // Adjust for odd number of elements

    // Split the file into two sorted chunks
    split(halfCount, fd, temp1, file_name);
    split(otherHalfCount, fd, temp2, file_name);
    close(fd);  // Close the source file

    // Merge the two sorted temporary files into the main file
    merge(temp1, temp2, halfCount, otherHalfCount, file_name);f
    close_all();  // Close all file descriptors

    // Clean up temporary files
    if (unlink(temp1) == -1 || unlink(temp2) == -1) {
        err(6, "Could not unlink temporary files");
    }
}

// Main entry point
int main(int argc, char** argv) {
    // Check for the correct number of arguments
    if (argc != 2) {
        errx(1, "Usage: %s <file_name>\n", argv[0]);
    }

    const char* file_name = argv[1];  // Get the file name from the argument
    sortFile(file_name);  // Sort the file
}
