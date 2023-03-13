#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{

    // equivalent of $ cat input.txt|grep COMP8567|wc|wc -w

    //  file descriptors
    // We will need 2 fd each for 3 different pipes
    int fd[3][2];

    // create and verify each pipe
    for (int i = 0; i < 3; i++)
    {

        if (pipe(fd[i]) == -1)
        {

            puts("There was an error creating the pipe");

            exit(1);
        }
    }

    // first fork to handle the cat command
    if (fork() == 0)
    {

        dup2(fd[0][1], STDOUT_FILENO);

        close(fd[0][0]);

        close(fd[0][1]);

        execlp("cat", "cat", "input.txt", NULL);

        puts("Error encountered while executing command");

        exit(1);
    }

    // Pipes' implementation starts from here
    // second fork to handle grep command
    if (fork() == 0)
    {

        dup2(fd[0][0], STDIN_FILENO);

        dup2(fd[1][1], STDOUT_FILENO);

        close(fd[0][1]);

        close(fd[1][0]);

        execlp("grep", "grep", "COMP8567", NULL);

        puts("Error encountered while executing command");

        exit(1);
    }

    // third fork to handle wc command
    if (fork() == 0)
    {

        dup2(fd[1][0], STDIN_FILENO);

        dup2(fd[2][1], STDOUT_FILENO);

        close(fd[0][0]);

        close(fd[0][1]);

        close(fd[1][1]);

        close(fd[2][0]);

        close(fd[2][1]);

        execlp("wc", "wc", NULL);

        puts("Error encountered while executing command");

        exit(1);
    }

    // fourth fork to handle the last wc
    if (fork() == 0)
    {

        dup2(fd[2][0], STDIN_FILENO);

        close(fd[0][0]);

        close(fd[0][1]);

        close(fd[1][0]);

        close(fd[1][1]);

        close(fd[2][0]);

        close(fd[2][1]);

        execlp("wc", "wc", "-w", NULL);

        puts("Error encountered while executing command");

        close(fd[0][0]);

        close(fd[0][1]);

        close(fd[1][0]);

        close(fd[1][1]);

        close(fd[2][1]);

        return 0;
    }

    return 0;
}
