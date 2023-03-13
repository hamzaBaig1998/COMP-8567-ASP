#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int process_id = fork();

    if (process_id == 0)
    {
        // process id == 0 => Child Process
        int i;
        for (i = 0; i < 30; i++)
        {
            printf("Child process id: %d | parent process id: %d\n", getpid(), getppid());
            sleep(1);
        }
        exit(0);
    }
    else if (process_id > 0)
    {
        // process id > 0 => Parent Process
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            // If child process terminates after completing its processing
            printf("Child process terminated normally with exit status: %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // If child process is terminated before completing its processing
            printf("Child process was terminated by signal %d\n", WTERMSIG(status));
        }
    }
    else
    {
        // process id == -1 => Fork Failed
        puts("Fork failed!");
    }

    return 0;
}