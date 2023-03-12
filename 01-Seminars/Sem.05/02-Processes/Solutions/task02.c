#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
        if(argc != 2) {
                errx(1, "Error: Invalid number of arguments");
        }

        if(execlp("ls", "ls", argv[1], (char*)NULL) < 0) {
                err(2, "Error executing ls command with argument %s", argv[1]);
        }

        return 0;
}
