#include <unistd.h>
#include <err.h>

int main(void) {
        if(execlp("sleep", "sleep", "60", (char*)NULL) == -1) {
                err(1, "Error excling: Could not execute command sleep");
        }

        return 0;
}
