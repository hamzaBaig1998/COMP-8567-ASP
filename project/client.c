// github: https://github.com/hamzaBaig1998/COMP-8567-ASP/tree/main/project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

#define O_BINARY 0

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 8082      // Port number of the server
#define BUFFER_SIZE 1024      // Buffer size for sending data to server
#define MAX_PATH_LENGTH 4096
#define MAX_ARGS 10

FILE *fp;

void unzip_tar_file(char *tar_filename)
{
    char command[MAX_PATH_LENGTH + 10];
    sprintf(command, "tar -xzf %s -C %s", tar_filename, getenv("PWD"));
    int status = system(command);
    if (status == -1)
    {
        perror("Error executing tar command");
        exit(1);
    }
}

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error: Failed to create socket");
        exit(1);
    }

    if ((fp = open("file.zip", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0777)) < 0)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error: Failed to connect to server");
        exit(1);
    }

    printf("\nConnected to server: %s:%d\n", SERVER_IP, SERVER_PORT);
    char command[1000], server_reply[2000];
    char *args[MAX_ARGS];
    int num_args;
    // Loop to send and receive commands
    while (1)
    {
        // Prompt user for command
        printf("> (type 'quit' to quit): ");
        fgets(command, 1000, stdin);
        char temp[1000];
        sprintf(temp, "%s", command);
        // break raw command
        num_args = 0;
        args[num_args] = strtok(temp, " \n");
        while (args[num_args] != NULL && num_args < MAX_ARGS - 1)
        {
            num_args++;
            args[num_args] = strtok(NULL, " \n");
        }
        // print arguments
        // for (int i = 0; i < num_args; i++)
        // {
        //     printf("args[%d]: %s\n", i, args[i]);
        // }

        // command validation
        int command_valid_flag = 0;
        if (strcmp(args[0], "findfile") == 0)
        {
            printf("findfiles invoked\n");
            if (num_args != 2)
            {
                fprintf(stderr, "Usage: %s filename\n", args[0]);
            }
            else
            {
                command_valid_flag = 1;
            }
        }

        else if (strcmp(args[0], "sgetfiles") == 0)
        {
            printf("sgetfiles invoked\n");
            command_valid_flag = 1;
            if (num_args < 3 || num_args > 4)
            {
                printf("Usage: %s size1 size2 [-u]\n", args[0]);
                command_valid_flag = 0;
            }
            else
            {
                long size1 = atol(args[1]);
                long size2 = atol(args[2]);
                if (size1 < 0 || size2 < 0 || size1 > size2)
                {
                    printf("Invalid size parameters\n");
                    command_valid_flag = 0;
                }
            }
        }

        else if (strcmp(args[0], "dgetfiles") == 0)
        {
            printf("dgetfiles invoked\n");
            command_valid_flag = 1;
            if (num_args < 3 || num_args > 4)
            {
                printf("Usage: %s date1 date2 [-u]\n", args[0]);
                command_valid_flag = 0;
            }
            else
            {
                struct tm date1_tm, date2_tm;
                memset(&date1_tm, 0, sizeof(date1_tm));
                memset(&date2_tm, 0, sizeof(date2_tm));
                if (strptime(args[1], "%Y-%m-%d", &date1_tm) == NULL || strptime(args[2], "%Y-%m-%d", &date2_tm) == NULL)
                {
                    printf("Invalid date format: YYYY-MM-DD\n");
                    command_valid_flag = 0;
                }

                time_t date1 = mktime(&date1_tm);
                time_t date2 = mktime(&date2_tm);
                if (date1 == -1 || date2 == -1 || date1 > date2)
                {
                    printf("Invalid date parameters\n");
                    command_valid_flag = 0;
                }
            }
        }

        else if (strcmp(args[0], "getfiles") == 0)
        {
            printf("getfiles invoked\n");
            if (num_args < 2 || num_args > 8 || (num_args == 2 && strcmp(args[1], "-u") == 0))
            {
                printf("Usage: %s file1 [file2 ... file6] [-u]\n", args[0]);
            }
            else
            {
                command_valid_flag = 1;
            }
        }

        else if (strcmp(args[0], "gettargz") == 0)
        {
            printf("gettarz invoked\n");
            if (num_args < 2 || num_args > 9 || (num_args == 2 && strcmp(args[1], "-u") == 0))
            {
                printf("Usage: %s extension1 [extension2 ... extension6] [-u]\n", args[0]);
            }
            else
            {
                command_valid_flag = 1;
            }
        }

        else if (strcmp(args[0], "quit") == 0)
        {
            // printf("exit invoked\n");
            printf("Exiting...\n");
            break;
        }
        else
        {
            // strcpy(result, "Bad Request!, Command not supported by the server\n");
            printf("Bad Request! Command not supported by server\n");
        }

        if (command_valid_flag)
        {
            // Send command to server
            write(client_socket, command, strlen(command));

            // Check for exit command
            // if (strcmp(command, "quit\n") == 0)
            // {
            //     printf("Exiting...\n");
            //     break;
            // }

            // Receive message from server
            if (recv(client_socket, server_reply, 2000, 0) < 0)
            {
                printf("recv failed. Error");
                break;
            }
            printf("Server reply: %s\n", server_reply);
        }
    }

    // while (1)
    // {
    //     // Send data to the server
    //     // sprintf(buffer, "Hello from client");
    //     // printf("> ");
    //     // fgets(buffer, sizeof(buffer), stdin);
    //     // if (send(client_socket, buffer, strlen(buffer), 0) == -1)
    //     // {
    //     //     perror("Error: Failed to send data to server");
    //     //     exit(1);
    //     // }
    //     // // printf("You entered: %s", buffer);
    //     // // Receive data from the server
    //     // bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    //     // if (bytes_received == -1)
    //     // {
    //     //     perror("Error: Failed to receive data from server");
    //     //     exit(1);
    //     // }

    //     // while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
    //     // {
    //     //     if (write(fp, buffer, bytes_received) != bytes_received)
    //     //     {
    //     //         perror("Error writing to file");
    //     //         exit(1);
    //     //     }
    //     // }

    //     // printf("Received data from server: %s\n", buffer);
    //     char command[100];
    //     printf("> ");
    //     scanf("%s", command);

    //     if (strcmp(command, "quit") == 0)
    //     {
    //         break;
    //     }

    //     if (send(client_socket, buffer, strlen(command), 0) == -1)
    //     {
    //         perror("Error: Failed to send data to server");
    //         exit(1);
    //     }
    //     else
    //     {
    //         printf("Command sent to server\n");
    //     }
    // }
    // Close the client socket
    close(client_socket);

    return 0;
}
