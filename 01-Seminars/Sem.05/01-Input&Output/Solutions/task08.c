#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

const uint8_t fileCount = 2;
int fds[] = {-1, -1};

void close_all(void);
void copy_and_modify_file(const char*, const char*);

int main(void) {
	const char* source = "/etc/passwd";
	const char* dest = "./passwd";

	copy_and_modify_file(source, dest);

	return 0;
}

void copy_and_modify_file(const char* source, const char* dest) {
	if((fds[0] = open(source, O_RDONLY)) == -1) {
		err(1, "Error opening file %s", source);
	}

	if((fds[1] = open(dest, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
		close_all();
		err(2, "Error opening file %s", dest);
	}

	char symbol;
	int read_bytes;

	while((read_bytes = read(fds[0], &symbol, sizeof(symbol))) == sizeof(char)) {
		if(':' == symbol) {
			symbol = '?';
		}
		if(write(fds[1], &symbol, sizeof(symbol)) != sizeof(char)) {
			close_all();
			err(3, "Error writing to file %s", dest);
		}
	}

	if(read_bytes < 0) {
		close_all();
		err(4, "Error reading from file %s", source);
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
