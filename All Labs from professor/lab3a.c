#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {

    int child_pid = fork();
    int status;
     
    //Child
    if (child_pid == 0) {
        for (int i = 0; i < 30; i++) {
            printf("Child ID: %d, Parent ID: %d\n", getpid(), getppid());
            sleep(1);
        }
        exit(0);
    } 
    else if (child_pid<0)   
     printf("ERROR\n");
    else {
        // Parent
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child terminated normally with exit status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
