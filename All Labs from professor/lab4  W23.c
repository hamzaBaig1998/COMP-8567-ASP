#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    pid_t pid, pid1;
    int status;
    pid = fork();
    pid1 = fork();
    if (pid == -1 || pid1 == -1)
    {
        printf("Error: Failed to create first child process");
    }

    if (pid == 0 && pid1 != 0)
    {
        wait(&status);
        printf("Second running pid: %d %d\n", getpid(), WEXITSTATUS(status));
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            execv("/bin/ls", (char *[]){"ls", "-1", "/Users/mrbox/Documents/University/APS/Lab4/lab4", NULL});
        }
    }
    if (pid1 == 0 && pid == 0)
    {
        printf("Grandchild running pid: %d, ppid: %d\n", getpid(), getppid());
        if (chdir("/Users/mrbox/Documents/University/APS/Lab4/lab4") == -1)
        {
            printf("Error: Failed to change directory");
            exit(1);
        }
        int file = creat("sample.txt", 0777);
        if (file == -1)
        {
            printf("Error: Failed to create file");
            exit(1);
        }
        printf("File 'sample.txt' created\n");
    }
    if (pid1 == 0 && pid != 0)
    {
        printf("First running pid: %d \n", getpid());
        execl("/bin/ls", "ls", "-1", "/Users/mrbox/Documents/University/APS/Lab4/", (char *)NULL);
    }

    return 0;
}