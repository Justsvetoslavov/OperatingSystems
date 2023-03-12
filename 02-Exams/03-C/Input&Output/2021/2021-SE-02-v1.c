#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <err.h>
#include <errno.h>

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void close_all(void);
int safe_open_read(const char*);
int safe_open_creat(const char*);
void write_safe(int, uint8_t, const char*);
uint8_t manchester_decoder(uint16_t);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Error: Invalid number of arguments: %d", argc - 1);
	}

	const char* f1 = argv[1];
	const char* f2 = argv[2];
	
	fds[0] = safe_open_read(f1);
	fds[1] = safe_open_creat(f2);

	uint16_t num;
	int read_bytes;
	while((read_bytes = read(fds[0], &num, sizeof(num))) > 0) {
		uint8_t decoded_num = manchester_decoder(num);
		write_safe(fds[1], decoded_num, f2);
	}
	if(read_bytes < 0) {
		close_all();
		err(5, "Could not read from file %s", f1);
	}

	close_all();
	return 0;
}

uint8_t manchester_decoder(uint16_t number) {
    uint8_t result = 0;
    for (int i = 0; i < 16; i += 2) {
        bool bit = (number >> (14 - i)) & 0b11; // Extract two bits
        if (bit == 0b10) { // Assuming 10 represents logical 1
            result |= (1 << (7 - i/2));
        }
        // No action needed for 01 since result is initialized to 0
        // Add handling for invalid pairs if necessary
    }
    return result;
}

void write_safe(int fd, uint8_t number, const char* filename) {
	if(write(fd, &number, sizeof(number)) != sizeof(number)) {
		close_all();
		err(4, "Error writing to file %s", filename);
	}
}

int safe_open_creat(const char* filename) {
	int fd;
	if((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1) {
		close_all();
		err(3, "Could not open file %s", filename);
	}
	return fd;
}

int safe_open_read(const char* filename) {
	int fd;
	if((fd = open(filename, O_RDONLY)) == -1) {
		close_all();
		err(2, "Could not open file %s", filename);
	}
	return fd;
}

void close_all(void) {
	int tempError = errno;
	for(uint8_t i = 0; i < fileCount; ++i) {
		if(fds[i] >= 0) {
			close(fds[i]);
		}
	}
	errno = tempError;
}
