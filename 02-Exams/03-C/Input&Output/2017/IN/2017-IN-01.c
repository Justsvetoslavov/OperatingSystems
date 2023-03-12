#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>

typedef struct __attribute__((packed)) {
	uint16_t offset;
	uint8_t length;
	uint8_t reserved;
} triplet_t;

const uint8_t fileCount = 4;
int fds[] = {-1, -1, -1, -1};

void close_all(void);
int safe_open(const char*);
int safe_create(const char*);
void safe_lseek(int, uint16_t, int, const char*);
bool isLetter(uint8_t);

int main(int argc, char* argv[]) {
	if(argc != 5) {
		errx(1, "Invalid number of arguments: %d", argc - 1);
	}

	const char* f1_dat = argv[1];
	const char* f1_idx = argv[2];

	struct stat s1_dat, s1_idx;

	if(stat(f1_dat, &s1_dat) == -1) {
		err(2, "Could not stat file %s", f1_dat);
	}

	if(s1_dat.st_size == 0) {
		err(3, "File %s is empty", f1_dat);
	}

	if(stat(f1_idx, &s1_idx) == -1) {
		err(4, "Could not stat file %s", f1_idx);
	}

	if((s1_idx.st_size == 0) || (s1_idx.st_size % sizeof(triplet_t) != 0)) {
		errx(5, "File %s is not consistent", f1_idx);
	}

	fds[0] = safe_open(f1_dat);
	fds[1] = safe_open(f1_idx);

	const char* f2_dat = argv[3];
	const char* f2_idx = argv[4];

	fds[2] = safe_create(f2_dat);
	fds[3] = safe_create(f2_idx);

	// Copy strings to f2.dat
	triplet_t t;
	int read_bytes;
	while((read_bytes = read(fds[1], &t, sizeof(t))) > 0) {
		if(t.offset + t.length > s1_dat.st_size) {
			close_all();
			errx(8, "Files are incosistent");
		}
	
		uint8_t* str = (uint8_t*)malloc(t.length * sizeof(uint8_t));
		if(str == NULL) {
			close_all();
			errx(7, "Memory allocation failed");
		}
		
		safe_lseek(fds[0], t.offset, SEEK_SET, f1_dat); 
		if(read(fds[0], str, t.length) != t.length) {
			close_all();
			err(8, "Error while reading file %s", f1_dat);
		}
		
		if(!isLetter(str[0])) {
			free(str);
			continue;
		}
		
	
		if(write(fds[2], str, t.length) != t.length) {
			close_all();
			err(9, "Could not write to file %s", f2_dat);
		}

		if(write(fds[3], &t, sizeof(t)) != sizeof(t)) {
			close_all();
			err(9, "Could not write to file %s", f2_idx);
		}

		free(str);
	}

	if(read_bytes < 0) {
		close_all();
		err(10, "Could not read from file %s", f1_idx);
	} 
	
	close_all();
	return 0;
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

int safe_open(const char* filename) {
	int fd;
	if((fd = open(filename, O_RDONLY)) == -1) {
		close_all();
		err(4, "Could not open file %s", filename);
	}
	return fd;
}

int safe_create(const char* filename) {
	int fd;
	if((fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
		close_all();
		err(5, "Could not open file %s", filename);
	}
	return fd;
}

void safe_lseek(int fd, uint16_t offset, int flags, const char* file) {
	if(lseek(fd, offset, flags) == -1) {
		close_all();
		err(6, "Could not lseek file %s", file);
	}
}

bool isLetter(uint8_t symbol) {
	uint8_t start = 0x41;
	uint8_t end = 0x5A;
	return((start <= symbol) && (symbol <= end));
}
