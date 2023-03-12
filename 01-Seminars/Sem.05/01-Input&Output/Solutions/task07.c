#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>

void copy_file(int, const char*);

int main(int argc, char* argv[]) {
	if(argc < 3) {
		errx(1, "Error: invalid number of arguments: %d", argc - 1);
	}

	int fd_open = open(argv[1], O_RDONLY);
	if(fd_open == -1) {
		err(2, "Error opening file %s", argv[1]);
	}

	for(int i = 2; i < argc; i++) {
		copy_file(fd_open, argv[i]);
		if(lseek(fd_open, 0, SEEK_SET) == -1) {
			close(fd_open);
			err(4, "Error with lseek of file %s", argv[1]);
		}
	}

	close(fd_open);

	return 0;
}

void copy_file(int fd_source, const char* file) {
	int fd_dest = open(file, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd_dest == -1) {
		printf("Error opening file: %s", file);
		return;
	}

	char buff[4096];
	int read_bytes = 0;
	while((read_bytes = read(fd_source, buff, sizeof(buff))) > 0) {
		if(write(fd_dest, buff, read_bytes) != read_bytes) {
			printf("Error writing to file: %s", file);
			close(fd_dest);
			return;
		}
	}
	if(read_bytes < 0) {
		printf("Error reading from file");
		close(fd_dest);
		return;
	}

	close(fd_dest);
}
