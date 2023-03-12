#include <unistd.h>
#include <err.h>

int main(void) {
        if(execlp("date", "date", (char*) NULL) < 0) {
                err(1, "Could not execute command 'date'");
        }

        return 0;
}
