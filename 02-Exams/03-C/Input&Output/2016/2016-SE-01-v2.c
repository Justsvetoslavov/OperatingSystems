// Solution by Radoslav

#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>


int main(int argc, const char** argv) {
        if(argc != 2) {
                errx(1, "Expected 1 argument, but %d were given", argc - 1);
        }

        int fd = open(argv[1], O_RDWR);
        if(fd == -1) {
                err(2, "Could not open file %s", argv[1]);
        }

        uint64_t count[256];
        for(int i = 0; i < 256; i++) {
                count[i] = 0;
        }

        uint8_t buff;
        int errcode = 0;
        while((errcode = read(fd, &buff, sizeof(buff))) > 0) {
                count[buff]++;
        }
        if(errcode == -1) {
                err(2, "Could not read");
        }

        if(lseek(fd, 0, SEEK_SET) == -1) {
                err(3, "Could not lseek");
        }
        for(uint16_t i = 0; i < 256; i++) {
                for(uint64_t j = 0; j < count[i]; j++) {
                    uint8_t temp = (uint8_t)i;
                        if(write(fd, &temp, sizeof(temp)) == -1) {
                                err(4, "Could not write");
                        }
                }
        }

        close(fd);

        return 0;
}
