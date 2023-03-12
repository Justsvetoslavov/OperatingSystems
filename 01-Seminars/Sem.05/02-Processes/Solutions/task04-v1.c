#include <unistd.h>
#include <stdio.h>
#include <err.h>

int main(void) {
        int n = 100; // Counter for loop

        pid_t pid = fork(); // Child: fork returns 0
                            // Parent: fork returns pid of child ( > 0 )
        if(pid < 0) {
                err(1, "Could not fork");
        }

        // Do note that the output is undeterministic
        if(pid > 0) { // Parent (greater than 0)
                for(int i = 0; i < n; i++) {
                        dprintf(1, "%d: Yes\n", i);
                }
        } else { // Child (equal to 0)
                for(int i = 0; i < n; i++) {
                        dprintf(1, "%d: No\n", i);
                }
        }

        return 0;
}
