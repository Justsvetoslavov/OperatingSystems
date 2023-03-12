#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void copyfile(const char*, const char*);
void close_all(void);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Invalid number of arguments: %d", argc - 1);
	}

	copyfile(argv[1], argv[2]);

	return 0;
}

void copyfile(const char* source, const char* dest) {
	if((fds[0] = open(source, O_RDONLY)) == -1) {
		err(2, "Error opening file: %s", source);
	}

	if((fds[1] = open(dest, O_CREAT | O_TRUNC | O_RDWR, 0644))== -1) {
		close_all();
		err(3, "Error opening file: %s", dest);
	}

	char buffer[4096];
	int read_size;
	while((read_size = read(fds[0], buffer, sizeof(buffer))) > 0) {
		if(write(fds[1], buffer, read_size) != read_size) {
			close_all();
			err(4, "Error writing to file %s", dest);
		}
	}
	if(read_size < 0) {
		close_all();
		err(5, "Error reading file: %s", source);
	}

	close_all();
}

void close_all(void) {
	int tempError = errno;
	for(int i = 0; i < fileCount; ++i) {
		if(fds[i] >= 0) {
			close(fds[i]);
		}
	}
	errno = tempError;
}
