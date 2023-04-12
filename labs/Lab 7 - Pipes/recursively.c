#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int create_pipe(int n, int pipefd[]);

void pipe_commands(int n, char *commands[])
{
    // Base case: no more commands to execute
    if (n == 0)
    {
        return;
    }

    // Create a pipe for the current command's input
    int input_pipe[2];
    if (create_pipe(1, input_pipe) < 0)
    {
        fprintf(stderr, "Failed to create pipe\n");
        return;
    }

    // Fork the current command
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    { // Child process
        // Redirect input to the pipe
        if (dup2(input_pipe[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            return;
        }

        // Close unused pipe ends
        close(input_pipe[0]);
        close(input_pipe[1]);

        // Execute the command
        execlp(commands[0], commands[0], NULL);
        perror("exec");
        return;
    }

    // Recursively pipe the remaining commands
    char *remaining_commands[n];
    for (int i = 1; i < n; i++)
    {
        remaining_commands[i - 1] = commands[i];
    }
    remaining_commands[n - 1] = NULL;
    pipe_commands(n - 1, remaining_commands);

    // Create a pipe for the current command's output
    int output_pipe[2];
    if (create_pipe(1, output_pipe) < 0)
    {
        fprintf(stderr, "Failed to create pipe\n");
        return;
    }

    // Fork the process for the next command
    pid_t next_pid;
    if ((next_pid = fork()) < 0)
    {
        perror("fork");
        return;
    }
    else if (next_pid == 0)
    { // Child process
        // Redirect output to the pipe
        if (dup2(output_pipe[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            return;
        }

        // Close unused pipe ends
        close(output_pipe[0]);
        close(output_pipe[1]);
        close(input_pipe[0]);
        close(input_pipe[1]);

        // Execute the command
        execlp(commands[n - 1], commands[n - 1], NULL);
        perror("exec");
        return;
    }

    // Close unused pipe ends in parent process
    close(input_pipe[0]);
    close(input_pipe[1]);
    close(output_pipe[1]);

    // Wait for child processes to complete
    waitpid(pid, NULL, 0);
    waitpid(next_pid, NULL, 0);
}

int create_pipe(int n, int pipefd[])
{
    if (pipe(pipefd) < 0)
    {
        return -1;
    }

    if (n > 1)
    {
        if (create_pipe(n - 1, pipefd + 2) < 0)
        {
            return -1;
        }
    }

    return 0;
}
int main(int argc, char *argv[])
{
    char *commands[] = {"cat", "input.txt", "wc", "wc", "-c", NULL};
    pipe_commands(5, commands);

    return 0;
}
