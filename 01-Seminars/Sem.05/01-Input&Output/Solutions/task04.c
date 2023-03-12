#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>

const uint8_t fileCount = 3; // Number of files/arguments
int fds[] = {-1, -1, -1}; // Global file descriptors

void swap_files(const char*, const char*);
void close_all(void);
int open_safe(const char*);
int mkstemp_safe(const char*);
void copy_content(int, const char*, int, const char*);
void safe_lseek(int, off_t, int, const char*);
void truncate_safe(int, const char*);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Invalid number of arguments: %d", argc - 1);
	}

	swap_files(argv[1], argv[2]);

	return 0;
}

void swap_files(const char* firstFile, const char* secondFile) {
	fds[0] = open_safe(firstFile);
	fds[1] = open_safe(secondFile);
	
	const char temporaryFileName[] = "my_temp_file_XXXXXX";
	fds[2] = mkstemp_safe(temporaryFileName);
	copy_content(fds[0], firstFile, fds[2], temporaryFileName);

	truncate_safe(fds[0], firstFile);
	copy_content(fds[1], secondFile, fds[0], firstFile);

	truncate_safe(fds[1], secondFile);
	safe_lseek(fds[2], 0, SEEK_SET, temporaryFileName);
	copy_content(fds[2], temporaryFileName, fds[1], secondFile);

	close_all();

	if(unlink(temporaryFileName) == -1) {
		err(8, "Error while unlinking file %s", temporaryFileName);
	}
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

int open_safe(const char* fileName) {
	int fd;
	if ((fd = open(fileName, O_RDWR | O_CREAT, 0644)) == -1) {
		close_all();
		err(2, "Error opening file %s", fileName);
	}

	return fd;
}

int mkstemp_safe(const char* fileName) {
	int fd;
	if((fd = mkstemp(fileName)) == -1) {
		close_all();
		err(3, "Error creating temporary file %s", fileName);
	}

	return fd;
}

void copy_content(int fdSource, const char* sourceName, int fdDest, const char* destName) {
	char buff[4096];
	int read_size;

	while((read_size = read(fdSource, buff, sizeof(buff))) > 0) {
		if(write(fdDest, buff, read_size) != read_size) {
			close_all();
			err(4, "Error writing to file %s", destName);
		}
	}
	if(read_size < 0) {
		close_all();
		err(5, "Error reading from file %s", sourceName);
	}
}

void safe_lseek(int fd, off_t position, int flags, const char* fileName) {
	if(lseek(fd, position, flags) == -1) {
		close_all();
		err(6, "Error while lseek in file %s", fileName);
	}
}

void truncate_safe(int fd, const char* fileName) {
	safe_lseek(fd, 0, SEEK_SET, fileName);
	if(ftruncate(fd, 0) == -1) {
		close_all();
		err(7, "Error while truncating file %s", fileName);
	}
}
