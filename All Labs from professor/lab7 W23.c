#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd1[2], fd2[2], fd3[2];
    pid_t pid1, pid2, pid3;

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    pid1 = fork();
    if (pid1 < 0)
    {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    if (pid1 == 0)
    {
        // Child process 1
        close(fd1[0]);
        dup2(fd1[1], STDOUT_FILENO);
        execl("/usr/bin/cat", "cat", "input.txt", NULL);
    }
    else
    {
        if (pipe(fd2) == -1)
        {
            fprintf(stderr, "Pipe failed");
            return 1;
        }
        pid2 = fork();
        if (pid2 < 0)
        {
            fprintf(stderr, "Fork failed");
            return 1;
        }
        if (pid2 == 0)
        {
            // Child process 2
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd1[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);
            execl("/bin/grep", "grep", "COMP8567", NULL);
        }
        else
        {
            if (pipe(fd3) == -1)
            {
                fprintf(stderr, "Pipe failed");
                return 1;
            }
            pid3 = fork();
            if (pid3 < 0)
            {
                fprintf(stderr, "Fork failed");
                return 1;
            }
            if (pid3 == 0)
            {
                // Child process 3
                close(fd2[1]);
                close(fd3[0]);
                dup2(fd2[0], STDIN_FILENO);
                dup2(fd3[1], STDOUT_FILENO);
                execl("/usr/bin/wc", "wc", NULL);
            }
            else
            {
                // Parent process
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
                close(fd3[1]);
                dup2(fd3[0], STDIN_FILENO);
                execl("/usr/bin/wc", "wc", "-w", NULL);
            }
        }
    }
    return 0;
}