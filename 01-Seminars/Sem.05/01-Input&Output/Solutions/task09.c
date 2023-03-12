#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

const char* min = "--min";
const char* max = "--max";
const char* print = "--print";

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Error: Invalid number of arguments: %d", argc - 1);
	}

	const char* option = argv[1];
	
	if((strcmp(option, min) != 0) && (strcmp(option, max) != 0) && (strcmp(option, print) != 0)) {
		errx(2, "Error: Invalid option: %s", option);
	}
	const char* filename = argv[2];
	struct stat st;
	if(stat(filename, &st) == -1) {
		err(3, "Error while stat file %s", filename);
	}

	if(st.st_size == 0) {
		errx(4, "File %s is empty", filename);
	}

	if((st.st_size % sizeof(uint16_t)) != 0) {
		errx(5, "File %s does not contain only uint16_t elements", filename);
	}

	int fd;
	if((fd = open(filename, O_RDONLY)) == -1) {
		err(6, "Error opening file %s", filename);
	}
	
	int bytes_read = 0;
	uint16_t number = 0;
	if(0 == strcmp(option, min)) {
		uint16_t min_number;
		if(read(fd, &min_number, sizeof(min_number)) == -1) {
			close(fd);
			err(4, "Error reading from file %s", filename);
		}
		while((bytes_read = read(fd, &number, sizeof(uint16_t))) == sizeof(uint16_t)) {
			if(number < min_number) {
				min_number = number;
			}
		}
		printf("Min number in file %s is %d\n", filename, min_number);
	} else if(0 == strcmp(option, max)) {
		uint16_t max_number;
		if(read(fd, &max_number, sizeof(uint16_t)) == -1) {
			close(fd);
			err(4, "Error reading from file %s", filename);
		}
		while((bytes_read = read(fd, &number, sizeof(uint16_t))) == sizeof(uint16_t)) {
			if(max_number < number) {
				max_number = number;
			}
		}
		printf("Max number in file %s is %d\n", filename, max_number);
	} else if(0 == strcmp(option, print)) {
		while((bytes_read = read(fd, &number, sizeof(uint16_t))) == sizeof(uint16_t)) {
		    dprintf(1, "%d\n", number);
		}
	}
	if(bytes_read < 0) {
		close(fd);
		err(6, "Error reading from file %s", filename);
	}

	close(fd);

	return 0;
}

