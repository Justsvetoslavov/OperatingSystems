#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

void head_func(const char*);
void close_safe(int fd);

int main(int argc, char* argv[]) {
	if(argc != 2)  {
		errx(1, "Error: Invalid number of arguments: %d", argc -1);
	}

	head_func(argv[1]);

	return 0;
}

void close_safe(int fd) { 
	if(fd >= 0) {
		int tempError = errno;
		close(fd);
		errno = tempError;
	}
}

void head_func(const char* file) {
	int fd = open(file, O_RDONLY);
	if(fd == -1) {
		err(2, "Error opening file %s", file);
	}

	char symbol;
	int counter = 0;
	int bytes_counter = 0;
	while((bytes_counter = read(fd, &symbol, sizeof(symbol))) == sizeof(char)) {
		if((write(1, &symbol, sizeof(symbol))) != 1) {
			close_safe(fd);
			err(3, "Error writing to stdout");
		}
		if (symbol == '\n') {
			++counter;
			if(counter >= 10) {
				break;
			}
		}
	}

	if(bytes_counter == -1) {
		close_safe(fd);
		err(4, "Error while reading file");
	}
	
	close_safe(fd);
}
