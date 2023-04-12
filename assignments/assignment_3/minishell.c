#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

// maximum command length
#define MAX_CMD_LENGTH 1024

int main_pipe[2];
int command_length;
char parsed_commands[6][200];
int totalCommands;

void split_command(char *cmd[50], char *split_cmd)
{
    int len = 0;
    char *ptr = strtok(split_cmd, " ");
    while (1)
    {
        if (ptr == NULL)
        {
            cmd[len] = NULL;
            len++;
            break;
        }
        cmd[len] = ptr;
        len++;
        ptr = strtok(NULL, " ");
    }

    if (len < 1 || len > 6)
    {
        puts("command arguments should be greater than equal to 1 and less than equal to 6");
        exit(10);
    }
}

void pipe_cmd(char *rawCommand)
{
    int len = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '|')
        {
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    parsed_commands[totalCommands][len] = '\0';
    totalCommands++;
    if (totalCommands < 1 || totalCommands > 6)
    {
        char message[100] = "Allowed total commands for pipe is >=1 and <=6\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(12);
    }

    int child_pipe_one[2];
    int child_pipe_two[2];
    int child_pipe_three[2];
    int child_pipe_four[2];
    int child_pipe_five[2];

    if (pipe(child_pipe_one) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }

    if (pipe(child_pipe_one) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }
    if (pipe(child_pipe_two) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }
    if (pipe(child_pipe_three) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }
    if (pipe(child_pipe_four) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }
    if (pipe(child_pipe_five) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }

    int totalCommandstemp = 0;
    if ((totalCommandstemp + 1) == totalCommands)
    {
        // First command
        char *command[50];
        split_command(command, parsed_commands[0]);
        dup2(main_pipe[1], 1);
        close(child_pipe_one[0]);
        close(child_pipe_one[1]);
        close(child_pipe_two[0]);
        close(child_pipe_two[1]);
        close(child_pipe_three[0]);
        close(child_pipe_three[1]);
        close(child_pipe_four[0]);
        close(child_pipe_four[1]);
        close(child_pipe_five[0]);
        close(child_pipe_five[1]);
        close(main_pipe[0]);
        close(main_pipe[1]);
        execvp(command[0], command);
        fprintf(stderr, "Error in command 1\n");
        exit(123);
    }
    else
    {
        totalCommandstemp++;
        int pid_1 = fork();
        if (pid_1 == 0)
        {
            // child 1
            char *command[50];
            split_command(command, parsed_commands[0]);
            close(1);
            dup2(child_pipe_one[1], 1);
            close(child_pipe_one[0]);
            close(child_pipe_one[1]);
            close(child_pipe_two[0]);
            close(child_pipe_two[1]);
            close(child_pipe_three[0]);
            close(child_pipe_three[1]);
            close(child_pipe_four[0]);
            close(child_pipe_four[1]);
            close(child_pipe_five[0]);
            close(child_pipe_five[1]);
            close(main_pipe[0]);
            close(main_pipe[1]);
            execvp(command[0], command);
            fprintf(stderr, "Error in command 1 child\n");
            exit(123);
        }
        else if (pid_1 > 0)
        {
            // wait(NULL);
            totalCommandstemp++;
            if (totalCommandstemp == totalCommands)
            {
                // child 1 last command
                char *command[50];
                split_command(command, parsed_commands[1]);
                dup2(child_pipe_one[0], 0);
                dup2(main_pipe[1], 1);
                close(child_pipe_one[0]);
                close(child_pipe_one[1]);
                close(child_pipe_two[0]);
                close(child_pipe_two[1]);
                close(child_pipe_three[0]);
                close(child_pipe_three[1]);
                close(child_pipe_four[0]);
                close(child_pipe_four[1]);
                close(child_pipe_five[0]);
                close(child_pipe_five[1]);
                close(main_pipe[0]);
                close(main_pipe[1]);

                execvp(command[0], command);
                fprintf(stderr, "Error in command 2\n");
                exit(123);
            }
            else
            {
                int pid_2 = fork();
                if (pid_2 == 0)
                {
                    // child 2
                    char *command[50];
                    split_command(command, parsed_commands[1]);
                    close(0);
                    close(1);
                    dup2(child_pipe_one[0], 0);
                    dup2(child_pipe_two[1], 1);
                    close(child_pipe_one[0]);
                    close(child_pipe_one[1]);
                    close(child_pipe_two[0]);
                    close(child_pipe_two[1]);
                    close(child_pipe_three[0]);
                    close(child_pipe_three[1]);
                    close(child_pipe_four[0]);
                    close(child_pipe_four[1]);
                    close(child_pipe_five[0]);
                    close(child_pipe_five[1]);
                    close(main_pipe[0]);
                    close(main_pipe[1]);
                    execvp(command[0], command);
                    fprintf(stderr, "Error in command 2 child\n");
                    exit(123);
                }
                else if (pid_2 > 0)
                {
                    // wait(NULL);
                    totalCommandstemp++;
                    if (totalCommandstemp == totalCommands)
                    {
                        printf("running last command\n");
                        // child 2 last command
                        char *command[50];
                        split_command(command, parsed_commands[2]);
                        close(0);
                        close(1);
                        dup2(child_pipe_two[0], 0);
                        dup2(main_pipe[1], 1);
                        close(child_pipe_one[0]);
                        close(child_pipe_one[1]);
                        close(child_pipe_two[0]);
                        close(child_pipe_two[1]);
                        close(child_pipe_three[0]);
                        close(child_pipe_three[1]);
                        close(child_pipe_four[0]);
                        close(child_pipe_four[1]);
                        close(child_pipe_five[0]);
                        close(child_pipe_five[1]);
                        close(main_pipe[0]);
                        close(main_pipe[1]);
                        execvp(command[0], command);
                        fprintf(stderr, "Error in command 3\n");
                        exit(123);
                    }
                    else
                    {
                        int pid_3 = fork();
                        if (pid_3 == 0)
                        {
                            // child 3
                            char *command[50];
                            split_command(command, parsed_commands[2]);
                            dup2(child_pipe_two[0], 0);
                            dup2(child_pipe_three[1], 1);
                            close(child_pipe_one[0]);
                            close(child_pipe_one[1]);
                            close(child_pipe_two[0]);
                            close(child_pipe_two[1]);
                            close(child_pipe_three[0]);
                            close(child_pipe_three[1]);
                            close(child_pipe_four[0]);
                            close(child_pipe_four[1]);
                            close(child_pipe_five[0]);
                            close(child_pipe_five[1]);
                            close(main_pipe[0]);
                            close(main_pipe[1]);
                            execvp(command[0], command);
                            fprintf(stderr, "Error in command 3 child\n");
                            exit(123);
                        }
                        else if (pid_3 > 0)
                        {
                            // wait(NULL);
                            totalCommandstemp++;
                            if (totalCommandstemp == totalCommands)
                            {
                                // child 3 last command
                                char *command[50];
                                split_command(command, parsed_commands[3]);
                                dup2(child_pipe_three[0], 0);
                                dup2(main_pipe[1], 1);
                                close(child_pipe_one[0]);
                                close(child_pipe_one[1]);
                                close(child_pipe_two[0]);
                                close(child_pipe_two[1]);
                                close(child_pipe_three[0]);
                                close(child_pipe_three[1]);
                                close(child_pipe_four[0]);
                                close(child_pipe_four[1]);
                                close(child_pipe_five[0]);
                                close(child_pipe_five[1]);
                                close(main_pipe[0]);
                                close(main_pipe[1]);
                                execvp(command[0], command);
                                fprintf(stderr, "Error in command 4\n");
                                exit(123);
                            }
                            else
                            {
                                int pid_4 = fork();
                                if (pid_4 == 0)
                                {
                                    // child 4
                                    char *command[50];
                                    split_command(command, parsed_commands[3]);
                                    dup2(child_pipe_three[0], 0);
                                    dup2(child_pipe_four[1], 1);
                                    close(child_pipe_one[0]);
                                    close(child_pipe_one[1]);
                                    close(child_pipe_two[0]);
                                    close(child_pipe_two[1]);
                                    close(child_pipe_three[0]);
                                    close(child_pipe_three[1]);
                                    close(child_pipe_four[0]);
                                    close(child_pipe_four[1]);
                                    close(child_pipe_five[0]);
                                    close(child_pipe_five[1]);
                                    close(main_pipe[0]);
                                    close(main_pipe[1]);
                                    execvp(command[0], command);
                                    fprintf(stderr, "Error in command 4 child\n");
                                    exit(123);
                                }
                                else if (pid_4 > 0)
                                {
                                    // wait(NULL);
                                    totalCommandstemp++;
                                    if (totalCommandstemp == totalCommands)
                                    {
                                        // child 4 last command
                                        char *command[50];
                                        split_command(command, parsed_commands[4]);
                                        dup2(child_pipe_four[0], 0);
                                        dup2(main_pipe[1], 1);
                                        close(child_pipe_one[0]);
                                        close(child_pipe_one[1]);
                                        close(child_pipe_two[0]);
                                        close(child_pipe_two[1]);
                                        close(child_pipe_three[0]);
                                        close(child_pipe_three[1]);
                                        close(child_pipe_four[0]);
                                        close(child_pipe_four[1]);
                                        close(child_pipe_five[0]);
                                        close(child_pipe_five[1]);
                                        close(main_pipe[0]);
                                        close(main_pipe[1]);
                                        execvp(command[0], command);
                                        fprintf(stderr, "Error in command 5\n");
                                        exit(123);
                                    }
                                    else
                                    {
                                        int pid_5 = fork();
                                        if (pid_5 == 0)
                                        {
                                            // child 5
                                            char *command[50];
                                            split_command(command, parsed_commands[4]);
                                            dup2(child_pipe_four[0], 0);
                                            dup2(child_pipe_five[1], 1);
                                            close(child_pipe_one[0]);
                                            close(child_pipe_one[1]);
                                            close(child_pipe_two[0]);
                                            close(child_pipe_two[1]);
                                            close(child_pipe_three[0]);
                                            close(child_pipe_three[1]);
                                            close(child_pipe_four[0]);
                                            close(child_pipe_four[1]);
                                            close(child_pipe_five[0]);
                                            close(child_pipe_five[1]);
                                            close(main_pipe[0]);
                                            close(main_pipe[1]);
                                            execvp(command[0], command);
                                            fprintf(stderr, "Error in command 5 child\n");
                                            exit(123);
                                        }
                                        else if (pid_5 > 0)
                                        {
                                            // wait(NULL);
                                            totalCommandstemp++;
                                            if (totalCommandstemp == totalCommands)
                                            {
                                                // child 5 last command
                                                char *command[50];
                                                split_command(command, parsed_commands[5]);
                                                dup2(child_pipe_five[0], 0);
                                                dup2(main_pipe[1], 1);
                                                close(child_pipe_one[0]);
                                                close(child_pipe_one[1]);
                                                close(child_pipe_two[0]);
                                                close(child_pipe_two[1]);
                                                close(child_pipe_three[0]);
                                                close(child_pipe_three[1]);
                                                close(child_pipe_four[0]);
                                                close(child_pipe_four[1]);
                                                close(child_pipe_five[0]);
                                                close(child_pipe_five[1]);
                                                close(main_pipe[0]);
                                                close(main_pipe[1]);
                                                execvp(command[0], command);
                                                fprintf(stderr, "Error in command 6\n");
                                                exit(123);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Flag = 0, Redirect to file with truncate;
// Flag = 1, Redirect to file with Append;
void right_redirect_cmd(char *rawCommand, int flag)
{
    int len = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '>')
        {
            if (rawCommand[i + 1] == '>')
            {
                i++;
            }
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    parsed_commands[totalCommands][len] = '\0';
    totalCommands++;

    if (totalCommands != 2)
    {
        char message[100] = "Allowed total parsed_commands for Redirect is 2\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(12);
    }

    int fd;
    int pid = fork();
    if (pid == 0)
    {
        char *command1[50];
        char *command2[50];
        split_command(command1, parsed_commands[0]);
        split_command(command2, parsed_commands[1]);
        int flag1 = O_WRONLY | O_CREAT | (flag == 0 ? O_TRUNC : O_APPEND);
        fd = open(command2[0], flag1, 0644);
        dup2(fd, STDOUT_FILENO);
        close(main_pipe[0]);
        close(main_pipe[1]);
        close(fd);
        execvp(command1[0], command1);
        // execvp() only returns if an error occurs
        fprintf(stderr, "Error Running command");
        exit(EXIT_FAILURE);
    }
    else
    {
        char message[50] = "Command Success";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(EXIT_FAILURE);
    }
}

void left_redirect_cmd(char *rawCommand)
{
    int len = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '<')
        {
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    parsed_commands[totalCommands][len] = '\0';
    totalCommands++;

    if (totalCommands != 2)
    {
        char message[100] = "Allowed total parsed_commands for Redirect is 2\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(12);
    }

    int fd;
    int pipe1[2];
    if (pipe(pipe1) == -1)
    {
        printf("init failed for child Pipe\n");
        exit(0);
    }
    int pid = fork();
    if (pid == 0)
    {
        char *command1[50];
        char *command2[50];
        split_command(command1, parsed_commands[0]);
        split_command(command2, parsed_commands[1]);
        fd = open(command2[0], O_RDONLY);
        dup2(fd, 0);
        dup2(main_pipe[1], 1);
        close(main_pipe[0]);
        close(main_pipe[1]);
        close(pipe1[0]);
        close(pipe1[1]);
        close(fd);
        execvp(command1[0], command1);
        fprintf(stderr, "Error Running command");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        close(main_pipe[0]);
        close(main_pipe[1]);
        close(pipe1[0]);
        close(pipe1[1]);
        wait(NULL);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
}

void background_cmd(char *rawCommand)
{
    int len = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '&')
        {
            if ((i + 1) != command_length)
            {
                char message[100] = "Allowed total parsed_commands for background is 1, Try removing extra spaces\n";
                write(main_pipe[1], message, strlen(message));
                close(main_pipe[0]);
                close(main_pipe[1]);
                exit(12);
            }
            parsed_commands[totalCommands][len] = ' ';
            parsed_commands[totalCommands][len + 1] = '&';
            parsed_commands[totalCommands][len + 2] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    int pid = fork();
    if (pid == 0)
    {
        char *command1[50];
        split_command(command1, parsed_commands[0]);
        // int c = 0;
        // while(command1[c] != NULL) {
        //  printf("%s ", command1[c]);
        //  c++;
        // }
        printf("\n");
        // freopen("output.txt", "w", stdout);
        close(0);
        close(1);
        close(main_pipe[0]);
        close(main_pipe[1]);
        execvp(command1[0], command1);
        // fprintf(stderr, "Error Running command");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        char message[100] = "Command running in background now\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        wait(NULL);
        exit(12);
    }
    exit(EXIT_FAILURE);
}

void sequential_cmd(char *rawCommand)
{
    int len = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == ';')
        {
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    parsed_commands[totalCommands][len] = '\0';
    totalCommands++;

    if (totalCommands < 1 || totalCommands > 5)
    {
        char message[100] = "Allowed total parsed_commands for Sequential is >=1 and <=5>\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(12);
    }

    int status;
    for (int i = 0; i < totalCommands; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            char *command[50];
            split_command(command, parsed_commands[i]);
            execvp(command[0], command);
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                char message[1000];

                snprintf(message, sizeof message, "\nCommand '%s' and number %d failed with status %d\n",
                         parsed_commands[i], i, WEXITSTATUS(status));
                write(main_pipe[1], message, strlen(message));
                close(main_pipe[0]);
                close(main_pipe[1]);
                exit(1);
            }
        }
    }

    char message[100] = "\nAll commands success\n";
    write(main_pipe[1], message, strlen(message));
    close(main_pipe[0]);
    close(main_pipe[1]);
    exit(123);
}

void and_or_cmd(char *rawCommand)
{
    int len = 0;
    int operatorBuffer[6] = {0};
    int j = 0;
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '&')
        {
            j++;
            if (rawCommand[i + 1] != '&')
            {
                char message[100] = "Invalid Command, should be double &&\n";
                write(main_pipe[1], message, strlen(message));
                close(main_pipe[0]);
                close(main_pipe[1]);
                exit(12);
            }
            i++;
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else if (rawCommand[i] == '|')
        {
            j++;
            operatorBuffer[j] = 1;
            if (rawCommand[i + 1] != '|')
            {
                char message[100] = "Invalid Command should be double ||\n";
                write(main_pipe[1], message, strlen(message));
                close(main_pipe[0]);
                close(main_pipe[1]);
                exit(12);
            }
            i++;
            parsed_commands[totalCommands][len] = '\0';
            totalCommands++;
            len = 0;
        }
        else
        {
            parsed_commands[totalCommands][len] = rawCommand[i];
            len++;
        }
    }

    parsed_commands[totalCommands][len] = '\0';
    totalCommands++;

    if (totalCommands < 1 || totalCommands > 5)
    {
        char message[100] = "Allowed total parsed_commands for conditional operator is >=1 and <=5>\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        exit(12);
    }

    int status;
    for (int i = 0; i < totalCommands; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            char *command[50];
            split_command(command, parsed_commands[i]);
            execvp(command[0], command);
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                char message[1000];
                snprintf(message, sizeof message, "\nCommand '%s' failed with status %d\n",
                         parsed_commands[i], i, WEXITSTATUS(status));

                if (i < 5 && operatorBuffer[i] == 0)
                {
                    write(main_pipe[1], message, strlen(message));
                    close(main_pipe[0]);
                    close(main_pipe[1]);
                    exit(1);
                }
                printf("%s", message);
            }
            else if ((totalCommands - 1) == i)
            {
                char message[1000] = "All commands ran";
                write(main_pipe[1], message, strlen(message));
                close(main_pipe[0]);
                close(main_pipe[1]);
                exit(1);
            }
            else
            {
                if (operatorBuffer[i] == 1)
                {
                    char message[1000] = "All commands ran";
                    write(main_pipe[1], message, strlen(message));
                    close(main_pipe[0]);
                    close(main_pipe[1]);
                    exit(1);
                }
            }
        }
    }
}

void run_cmd(char *rawCommand)
{
    for (int i = 0; i < command_length; i++)
    {
        if (rawCommand[i] == '|')
        {
            if (rawCommand[i + 1] == '|')
            {
                and_or_cmd(rawCommand);
                exit(123);
            }

            pipe_cmd(rawCommand);
            exit(123);
        }

        if (rawCommand[i] == '&')
        {
            if (rawCommand[i + 1] == '&')
            {
                and_or_cmd(rawCommand);
                exit(123);
            }
            background_cmd(rawCommand);
            exit(123);
        }

        if (rawCommand[i] == '>')
        {
            if (rawCommand[i + 1] == '>')
            {
                right_redirect_cmd(rawCommand, 1);
                exit(123);
            }

            right_redirect_cmd(rawCommand, 0);
            exit(123);
        }

        if (rawCommand[i] == '<')
        {
            left_redirect_cmd(rawCommand);
            exit(123);
        }

        if (rawCommand[i] == ';')
        {
            sequential_cmd(rawCommand);
            exit(123);
        }
    }

    char *command[50];
    split_command(command, rawCommand);
    if (stderr)
    {
        fprintf(stderr, "Error in first command\n");
        char message[100] = "Error running command\n";
        write(main_pipe[1], message, strlen(message));
        close(main_pipe[0]);
        close(main_pipe[1]);
        wait(NULL);
    }
    else
    {
        dup2(main_pipe[1], 1);
        close(main_pipe[0]);
        close(main_pipe[1]);
    }
    execvp(command[0], command);
    exit(12);
}

int main()
{
    int current_process_id;
    char cmd[MAX_CMD_LENGTH];

    printf("use exit command to exit from the shell\n");
    while (1)
    {
        printf("ms$ ");
        fgets(cmd, MAX_CMD_LENGTH, stdin);

        // remove newline character from the end of the command string
        cmd[strcspn(cmd, "\n")] = '\0';

        // check if user wants to exit
        if (strcmp(cmd, "exit") == 0)
        {
            break;
        }

        // execute the command using system call
        // use fork() and exec() instead
        // int status = system(cmd);
        // if (status == -1)
        // {
        //     perror("Error executing command");
        // }
        // else
        // {
        //     printf("\n");
        // }

        command_length = strlen(cmd);
        current_process_id = fork();
        if (current_process_id == 0)
        {
            run_cmd(cmd);
            exit(123);
        }
        else if (current_process_id >= 1)
        {
            // Parent process waits for the child process to complete
            int status;

            char buffer[MAX_CMD_LENGTH];
            int n = read(main_pipe[0], buffer, MAX_CMD_LENGTH);
            buffer[n] = '\0';
            printf("%s\n", buffer);
        }
        else
        {
            fprintf(stderr, "child fork() failed.\n");
            exit(1);
        }
    }
    return 0;
}
