#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void close_all(void);
int safe_open(const char*);
int safe_creat(const char*);
void write_safe(int, const char*, const char*);
uint16_t read_num(int, const char*);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Error: Invalid number of arguments: %d", argc - 1);
	}
	
	const char* inputfile = argv[1];
	const char* outputfile = argv[2];

	struct stat s;
	if(stat(inputfile, &s) == -1) {
		err(2, "Could not stat file %s", inputfile);
	}
	uint32_t size = s.st_size / sizeof(uint16_t);
	if((size == 0) || (size > 524288) || (s.st_size % sizeof(uint16_t) != 0)) {
		err(3, "File %s is not consistent", inputfile);
	}
	
	fds[0] = safe_open(inputfile);
	fds[1] = safe_creat(outputfile);
	
	write_safe(fds[1], "#include <unistd.h>\n", outputfile);
	write_safe(fds[1], "#include <stdint.h>\n", outputfile);
	write_safe(fds[1], "uint32_t arrN = ", outputfile);
	dprintf(fds[1], "%u;\n", size);
	write_safe(fds[1], "const uint16_t arr[] = { ", outputfile);

	for(uint32_t i = 0; i < size - 1; ++i) {
		uint16_t number = read_num(fds[0], inputfile);
		dprintf(fds[1], "%hu, ", number);
	}

	uint16_t num = read_num(fds[0], inputfile);
	dprintf(fds[1], "%hu};\n", num);

	return 0;
}

uint16_t read_num(int fd, const char* filename) {
	uint16_t num;
	if(read(fd, &num, sizeof(num)) != sizeof(num)) {
		close_all();
		err(7, "Could not read from file %s", filename);
	}
	return num;
}

void write_safe(int fd, const char* str, const char* filename) {
	if(write(fd, str, strlen(str)) != (ssize_t)strlen(str)) {
		close_all();
		err(6, "Could not write to file %s", filename);
	}
}

int safe_creat(const char* filename) {
	int fd;
	if((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1) {
		close_all();
		err(5, "Could not open file %s", filename);
	}
	return fd;
}

int safe_open(const char* filename) {
	int fd;
	if((fd = open(filename, O_RDONLY)) == -1) {
		close_all();
		err(4, "Could not open file %s", filename);
	}
	return fd;
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
