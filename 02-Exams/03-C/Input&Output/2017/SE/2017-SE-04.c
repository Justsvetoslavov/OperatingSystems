#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <err.h>
#include <string.h>

void read_fd(int fd);

int main(int argc, char* argv[]) {
	if(argc == 1) {
		read_fd(0);
	} else {
		for(int i = 1; i < argc; ++i) {
			if(0 == strcmp(argv[i], "-")) {
				read_fd(0);
			} else {
				int fd = open(argv[i], O_RDONLY);
				if(fd == -1) {
					err(1, "Could not open file %s", argv[i]);
				}
				read_fd(fd);
				close(fd);
			}
		}

	}

	return 0;
}

void read_fd(int fd) {
	char s;
	int read_bytes;
	while((read_bytes = read(fd, &s, sizeof(s))) > 0) {
		if(write(1, &s, sizeof(s)) != sizeof(s)) {
			close(fd);
			err(2, "Could not writeto file");
		}
	}
	if(read_bytes < 0) {
		close(fd);
		err(3, "Could not read from file");
	}

	printf("\n"); //optional separator between different files
}
