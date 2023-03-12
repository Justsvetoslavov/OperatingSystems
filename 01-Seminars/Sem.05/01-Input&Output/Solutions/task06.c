#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

void cat_func(int, char**);

int main(int argc, char* argv[]) {
	if(argc <= 1) {
		errx(1, "Invalid number of arguments: %d", argc - 1);
	}

	cat_func(argc, argv);

	return 0;
}

void cat_func(int argc, char* argv[]) {
	int fd;
	char buffer[4096];
	int read_bytes;

	for(int i = 1; i < argc; ++i) {
		fd = open(argv[i], O_RDONLY);
		if(fd == -1) {
			printf("Error opening file: %s", argv[i]);
			continue;
		}

		while((read_bytes = read(fd, buffer, sizeof(buffer))) > 0) {
			if(write(1, buffer, read_bytes) != read_bytes) {
				close(fd);
				printf("Error while writing to stdout with file %s", argv[i]);
				continue;
			}
		}

		if(read_bytes < 0) {
			close(fd);
			dprintf(2, "Error while reading file: %s", argv[i]);
			continue;
		}
		
		buffer[0] = '\0';
		close(fd);
	}
}
