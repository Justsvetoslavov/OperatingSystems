#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

void wc_func(const char*);
void close_safe(int fd);

int main(int argc, char* argv[]) {
	if(argc != 2) {
		err(1, "Error invalid number of arguments: %d", argc - 1);
	}

	wc_func(argv[1]);

	return 0;
}

void close_safe(int fd) {
	if(fd >= 0) {
		int tempError = errno;
		close(fd);
		errno = tempError;
	}
}

void wc_func(const char* filename) {
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		err(2, "Error opening file: %s", filename);
	}

	int lines_count = 0;
	int word_count = 0;
	int char_count = 0;
    bool in_word = false;

	char ch;
	int bytes_read = 0;
	while((bytes_read = read(fd, &ch, sizeof(ch))) == sizeof(ch)) {
		if ('\n' == ch) {
			++lines_count;
		}

		if (' ' == ch || '\t' == ch || '\n' == ch) {
			in_word = false;
		} else if(!in_word) {
			in_word = true;
	    	++word_count;
 	    }

		++char_count;
	}

	if(bytes_read == -1) {
		close_safe(fd);
		err(3, "Error reading from file: %s", filename);
	}

	printf("File %s has:\n%d number of lines:\n%d number of words:\n%d number of characters:\n", filename, lines_count, word_count, char_count);

	close_safe(fd);
}
