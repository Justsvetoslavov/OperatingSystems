#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

int fd[] = {-1, -1}; // Global file descriptors
const uint8_t fileCount = 2; // Global count of arguments/files

void copy_file(const char*, const char*);
void close_all(void);

int main(int argc, char* argv[])  {
	if(argc != 3) {
		// We need exactly 2 arguments/files
		errx(1, "Invalid number of arguments: %d", argc - 1);
	}

	copy_file(argv[1], argv[2]);

	return 0;
}

void close_all(void) {
	int tempError = errno;
	for(int i = 0; i < fileCount; ++i) {
		if(fd[i] >= 0) {
			close(fd[i]);
		}
	}
	errno = tempError;
}

void copy_file(const char* sourceFile, const char* destFile) {
	// open the source file for reading
	if((fd[0] = open(sourceFile, O_RDONLY)) == -1) {
		err(1, "Error opening source file %s", sourceFile);
	}

	// Open destination file or create destination file
	if((fd[1] = open(destFile, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
		close_all();
		err(2, "Error opening destination file %s", destFile);
	}

	char buff[4096];
	int bytes;
	while((bytes = read(fd[0], buff, sizeof(buff))) > 0) {
		if(write(fd[1], buff, bytes) != bytes) {
			close_all();
			err(3, "Error writing in file %s", destFile);
		}
	}

	if(bytes < 0) {
		close_all();
		err(4, "Error reading from file %s", sourceFile);
	}
			
	close_all();
}
