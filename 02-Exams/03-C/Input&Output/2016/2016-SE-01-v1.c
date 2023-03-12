#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

void bubbleSort(unsigned char*, size_t);

void bubbleSort(unsigned char* arr, size_t size) {
        for(size_t i = 0; i < size - 1; ++i) {
                bool swapped = false;
                for(size_t j = 0; j < size - i - 1; ++j) {
                        if(arr[j] > arr[j+1]) {
                                unsigned char temp = arr[j];
                                arr[j] = arr[j + 1];
                                arr[j+1] = temp;
                                swapped = true;
                        }
                }
                if(!swapped) { // Exit early if no swaps
                        break;
                }
        }
}

int main(int argc, char* argv[]) {
        if(argc != 2) {
                errx(1, "Invalid argument count");
        }

        const char* filename = argv[1];
        int fd = open(filename, O_RDWR);
        if (fd < 0) {
                err(2, "Could not open file %s", filename);
        }

        struct stat fileStat;
        if(fstat(fd, &fileStat) < 0) {
                close(fd);
                err(3, "Could not stat file %s", filename);
        }
        ssize_t fileSize = fileStat.st_size;

        if(0 == fileSize) {
                close(fd);
                errx(4, "File %s is empty", filename);
        }
        unsigned char* buffer = (unsigned char*)malloc(fileSize);
        if( NULL == buffer ) {
                close(fd);
                errx(5, "Memory allocation failed");
        }

        if(fileSize != read(fd, buffer, fileSize)) {
                close(fd);
                free(buffer);
                err(5, "Error reading from file %s", filename);
        }

        bubbleSort(buffer, fileSize);

        if(lseek(fd, 0, SEEK_SET) < 0) {
                close(fd);
                free(buffer);
                err(6, "Could not lseek file %s", filename);
        }

        if(fileSize != write(fd, buffer, fileSize)) {
                close(fd);
                free(buffer);
                err(7, "Could not write to file %s", filename);
        }

        close(fd);
        free(buffer);
}
