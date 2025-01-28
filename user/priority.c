#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = fork();
    if (pid == 0) {
        setpriority(getpid(), 1);
        while (1) {
            printf("Child process running\n");
            sleep(10);
            break;
        }
    } else {
        setpriority(getpid(), 15);  // Parent with lower priority
        wait(&pid);
        while (1) {
            printf("Parent process running\n");
            sleep(10);
            break; 
        }
    }
    return 0;
}