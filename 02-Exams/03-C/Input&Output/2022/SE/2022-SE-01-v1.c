#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) {
	uint32_t magic;
	uint32_t count;
} data_header_t;

typedef struct __attribute__((packed)) {
	uint32_t magic_32;
	uint16_t magic_16;
	uint16_t reserved;
	uint64_t count;
} comparator_header_t;

typedef struct __attribute__((packed)) {
	uint16_t type; // 0 or 1
	uint16_t reserved[3]; // only 0
	uint32_t offset1;
	uint32_t offset2;
} comparator_data_t;

// Constants 
const uint32_t magic_data_bin = 0x21796F4A;
const uint32_t magic_data_comparator_32 = 0xAFBC7A37;
const uint16_t magic_data_comparator_16 = 0x1C27;

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void close_all(void);
int safe_open_rdwr(const char*);
int safe_open_read(const char*);
bool check_header_data(int);
bool check_comparator_header_data(int);
void safe_lseek(int, uint32_t, int, const char*);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Error: Invalid number of arguments: %d", argc - 1);
	}
	
	const char* datafile = argv[1]; // data.bin
	struct stat s_data;
	if(stat(datafile, &s_data) == -1) {
		err(2, "Could not stat file %s", datafile);
	}
	if( (s_data.st_size == 0) || (s_data.st_size < (off_t)sizeof(data_header_t)) || ( (s_data.st_size - (off_t)sizeof(data_header_t)) % sizeof(uint64_t) != 0)) {
		errx(3, "File %s is inconsistent", datafile);
	}
	
	const char* compfile = argv[2];
	struct stat s_comp;
	if(stat(compfile, &s_comp) == -1) {
		err(4, "Could not stat file %s", compfile);
	}
	if( (s_comp.st_size == 0) || (s_comp.st_size < (off_t)sizeof(comparator_header_t)) || ((s_comp.st_size - (off_t)sizeof(comparator_header_t)) % sizeof(comparator_data_t) != 0)) {
		errx(5, "File %s is inconsistent", compfile);
	}

	fds[0] = safe_open_rdwr(datafile);
	fds[1] = safe_open_read(compfile);
	
	if(!check_header_data(fds[0])) {
		close_all();
		errx(6, "Data header is not correct");
	}

	if(!check_comparator_header_data(fds[1])) {
		close_all();
		errx(7, "Comparator header is not correct");
	}

	int read_bytes;
	comparator_data_t data;
	while((read_bytes = read(fds[1], &data, sizeof(data))) == sizeof(data)) {
		if(data.type != 0 && data.type != 1) {
			close_all();
			errx(8, "Invalid comparator type");
		}
		uint64_t offset1 = data.offset1 * sizeof(uint64_t);
		uint64_t offset2 = data.offset2 * sizeof(uint64_t);
		if((offset1 > (uint64_t)s_data.st_size) || (offset2 > (uint64_t)s_data.st_size)) {
			close_all();
			errx(9, "Invalid offset in comparator");
		}

		if(data.reserved[0] == 0 || data.reserved[1] == 0 || data.reserved[2] == 0) {
			close_all();
			errx(10, "Invalid comparator data format");
		}
		

		safe_lseek(fds[0], offset1, SEEK_SET, datafile);
        uint64_t number1;
		if(read(fds[0], &number1, sizeof(number1)) != sizeof(number1)) {
			close_all();
			err(11, "Error reading from file %s", datafile);
		}

		safe_lseek(fds[0], offset2, SEEK_SET, datafile);
		uint64_t number2;
		if(read(fds[0], &number2, sizeof(number2)) != sizeof(number2)) {
			close_all();
			err(12, "Error reading from file %s", datafile);
		}
		

		if(data.type == 0) { // number1 is less than number2
			if(number1 >  number2) {
				safe_lseek(fds[0], offset1, SEEK_SET, datafile);
				if(write(fds[0], &number2, sizeof(number2)) != sizeof(number2)) {
					close_all();
					err(13, "Error writing to file %s", datafile);
				}

				safe_lseek(fds[0], offset2, SEEK_SET, datafile);
				if(write(fds[0], &number1, sizeof(number1)) != sizeof(number1)) {
					close_all();
					err(14, "Error writing to file %s", datafile);
				}
			}
		} else if (data.type == 1) {
			if(number1 < number2) {
				safe_lseek(fds[0], offset1, SEEK_SET, datafile);
				if(write(fds[0], &number2, sizeof(number2)) != sizeof(number2)) {
					close_all();
					err(13, "Error writing to file %s", datafile);
				}

				safe_lseek(fds[0], offset2, SEEK_SET, datafile);
				if(write(fds[0], &number1, sizeof(number1)) != sizeof(number1)) {
					close_all();
					err(14, "Error writing to file %s", datafile);
				}

			}
		}
		
	}
	if(read_bytes < 0) {
		close_all();
		err(11, "Error reading from file %s", compfile);
	}

	close_all();
	return 0;
}

void close_all(void) {
	int temp = errno;
	for(uint8_t i = 0; i < fileCount; ++i) {
		if(fds[i] >= 0) {
			close(fds[i]);
		}
	}
	errno = temp;
}

int safe_open_rdwr(const char* filename) {
	int fd;
	if((fd = open(filename, O_RDWR)) == -1) {
		close_all();
		err(6, "Could not open file %s", filename);
	}
	return fd;
}

int safe_open_read(const char* filename) {
	int fd;
	if((fd = open(filename, O_RDONLY)) == -1) {
		close_all();
		err(7, "Could not open file %s", filename);
	}
	return fd;
}

bool check_header_data(int fd) {
	data_header_t data;
	int read_bytes = read(fd, &data, sizeof(data));
	if(read_bytes != sizeof(data)) {
		close_all();
		err(8, "Could not read from data file");
	}

	if(data.magic != magic_data_bin) {
		return false;
	}

	if(data.count == 0) {
		close_all();
		printf("Element count is 0. Exiting\n");
		exit(0);
	}

	return true;
}

bool check_comparator_header_data(int fd) {
	comparator_header_t data;
	int read_bytes = read(fd, &data, sizeof(data));
	if(read_bytes != sizeof(data)) {
		close_all();
		err(9, "Could not read from comparator file");
	}

	if((data.magic_32 != magic_data_comparator_32) || (data.magic_16 != magic_data_comparator_16)) {
		return false;
	}

	if(data.count == 0) {
		close_all();
		printf("Comparator element count is 0. Exiting\n");
		exit(0);
	}

	return true;
}

void safe_lseek(int fd, uint32_t offset, int flags, const char* filename) {
	if(lseek(fd, offset, flags) == -1) {
		close_all();
		err(11, "Error lseeking file %s", filename);
	}
}


