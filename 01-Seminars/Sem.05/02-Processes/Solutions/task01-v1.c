#include <unistd.h>
#include <err.h>

int main(void) {
        if(execlp("/bin/date", "date", (char*)NULL) == -1) {
                err(1, "Error excling");
        }

        return 0;
}
