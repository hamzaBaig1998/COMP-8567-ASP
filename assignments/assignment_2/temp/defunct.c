#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        printf("Child process running\n");
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process
        printf("Parent process waiting for child\n");
        wait(NULL);
        printf("Parent process done waiting\n");
        while (1)
        {
            sleep(1);
        }
    }
    else
    {
        // Fork error
        printf("Fork failed\n");
        exit(1);
    }

    return 0;
}
