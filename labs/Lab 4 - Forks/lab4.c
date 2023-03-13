#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int pid1, pid2, pid3;

    // fork first child process
    pid1 = fork();
    pid2 = fork();
    // pid3 = fork();
    if (pid1 < 0)
    {
        printf("Error in creating first child process\n");
        return 1;
    }
    else if (pid1 == 0)
    {
        // fork second child process (grandchild)
        pid3 = fork();
        if (pid3 < 0)
        {
            printf("Error in creating second child process\n");
            return 1;
        }
        else if (pid3 == 0)
        {
            // change working directory to /home/hamza/lab4
            if (chdir("/home/hamza/lab4") != 0)
            {
                printf("Error in changing working directory\n");
                return 1;
            }

            // create sample.txt with 0777 permissions
            int file_descriptor = open("sample.txt", O_CREAT | O_RDWR, 0777);
            if (file_descriptor < 0)
            {
                printf("Error in creating file\n");
                return 1;
            }

            // close the file
            close(file_descriptor);
        }
        else
        {
            // wait for grandchild to finish execution
            int status;
            waitpid(pid3, &status, 0);

            // differentiate and print the contents of /home/hamza/lab4
            char *args[] = {"ls", "-1", "/home/hamza/lab4", NULL};
            execv("/bin/ls", args);
        }
    }
    else
    {
        // fork second child process (C2)
        if (pid2 < 0)
        {
            printf("Error in creating second child process\n");
            return 1;
        }
        else if (pid2 == 0)
        {
            // differentiate child 2 (C2) of the main process with ls -1 /home/hamza
            char *args[] = {"ls", "-1", "/home/hamza", NULL};
            execl("/bin/ls", "ls", "-1", "/home/hamza", (char *)NULL);
        }
        else
        {
            // wait for both children to complete execution
            int status1, status2;
            waitpid(pid1, &status1, 0);
            waitpid(pid2, &status2, 0);

            printf("Main process completed\n");
        }
    }

    return 0;
}