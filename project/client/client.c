// github: https://github.com/hamzaBaig1998/COMP-8567-ASP/tree/main/project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define O_BINARY 0

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 8082      // Port number of the server
#define BUFFER_SIZE 10000     // Buffer size for sending data to server
#define MAX_PATH_LENGTH 4096
#define MAX_ARGS 10

FILE *fp;

void unzip_tar_file(char *tar_filename)
{
    printf("unzipping file...\n");
    char command[MAX_PATH_LENGTH + 10];
    sprintf(command, "tar -xzf %s -C %s", tar_filename, getenv("PWD"));
    int status = system(command);
    if (status == -1)
    {
        perror("Error executing tar command");
        exit(1);
    }
    printf("File unzipped...\n");
}

// handle file recieve
void receive_file(int socketfd, int unzipProcess)
{
    int pid = fork();
    if (pid == 0)
    {
        char buffer[BUFFER_SIZE];
        char tarName[40];
        sprintf(tarName, "temp.tar.gz");

        // Open file
        int fd = open(tarName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Receive file contents
        ssize_t bytes_received = 1;
        long int total_bytes_received = 0;
        while (bytes_received > 0)
        {
            memset(buffer, 0, sizeof(buffer));
            bytes_received = recv(socketfd, buffer, BUFFER_SIZE, 0);
            if (bytes_received < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }

            long int bytes_written = write(fd, buffer, bytes_received);
            if (bytes_written < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
            memset(buffer, 0, sizeof(buffer));

            total_bytes_received += bytes_written;
        }

        if (total_bytes_received == 0)
        {
            printf("No files found.\n");
            // close(fd);
            // exit(0);
        }
        else
        {
            printf("File received successfully.\n");
            printf("Total file received %d\n", total_bytes_received);
        }
        // Clean up
        close(fd);
        exit(0);
    }
    else
    {
        sleep(10);
        kill(pid, SIGKILL);
        printf("parent received success\n");
        if (unzipProcess == 1)
        {
            unzip_tar_file("temp.tar.gz");
        }
    }
}

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    ssize_t read_bytes;
    int file_fd;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error: Failed to create socket");
        exit(1);
    }

    // if ((fp = open("file.zip", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0777)) < 0)
    // {
    //     perror("open failed");
    //     exit(EXIT_FAILURE);
    // }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == -1)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

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
        int file_flag = 0;
        int unzip = 0;
        if (strcmp(args[0], "findfile") == 0)
        {
            file_flag = 0;
            printf("findfiles invoked\n");
            if (num_args != 2)
            {
                fprintf(stderr, "Usage: %s filename\n", args[0]);
            }
            else
            {
                command_valid_flag = 1;
            }

            // send
            // if (command_valid_flag)
            // {
            //     // Send command to server
            //     write(client_socket, command, strlen(command));

            //     // Check for exit command
            //     // if (strcmp(command, "quit\n") == 0)
            //     // {
            //     //     printf("Exiting...\n");
            //     //     break;
            //     // }

            //     // Receive message from server
            //     memset(server_reply, 0, sizeof(server_reply));
            //     if (recv(client_socket, server_reply, 2000, 0) < 0)
            //     {
            //         printf("recv failed. Error");
            //         break;
            //     }
            //     printf("Server reply: %s\n", server_reply);
            //     memset(server_reply, 0, sizeof(server_reply));
            // }
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
            if (command_valid_flag)
            {
                file_flag = 1;
                if (strcmp(args[num_args - 1], "-u") == 0)
                {
                    unzip = 1;
                }
            }

            // // send command
            // if (command_valid_flag)
            // {
            //     // Send command to server
            //     write(client_socket, command, strlen(command));

            //     // if ((file_fd = open("temp.tar.gz", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
            //     // {
            //     //     perror("open");
            //     //     exit(EXIT_FAILURE);
            //     // }
            //     receive_file(client_socket, "temp.tar.gz");
            //     // // Reading and writing file content
            //     // while ((read_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
            //     // {
            //     //     if (write(file_fd, buffer, read_bytes) == -1)
            //     //     {
            //     //         perror("write");
            //     //         exit(EXIT_FAILURE);
            //     //     }
            //     // }

            //     // close(file_fd);
            //     // ssize_t bytes_received = 1;
            //     // long int total_bytes_received = 0;
            //     // while (bytes_received > 0)
            //     // {
            //     //     bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            //     //     if (bytes_received < 0)
            //     //     {
            //     //         perror("recv");
            //     //         exit(EXIT_FAILURE);
            //     //     }

            //     //     long int bytes_written = write(file_fd, buffer, bytes_received);
            //     //     if (bytes_written < 0)
            //     //     {
            //     //         perror("write");
            //     //         exit(EXIT_FAILURE);
            //     //     }

            //     //     total_bytes_received += bytes_written;
            //     // }

            //     // printf("File received successfully.\n");
            //     // printf("Total file received %d\n", total_bytes_received);
            //     // close(file_fd);
            // }
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
            if (command_valid_flag)
            {
                file_flag = 1;
                if (strcmp(args[num_args - 1], "-u") == 0)
                {
                    unzip = 1;
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
                file_flag = 1;
                command_valid_flag = 1;
                if (strcmp(args[num_args - 1], "-u") == 0)
                {
                    unzip = 1;
                }
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
                file_flag = 1;
                if (strcmp(args[num_args - 1], "-u") == 0)
                {
                    unzip = 1;
                }
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
            printf("Bad Request! Command not supported by server\n");
        }

        if (command_valid_flag)
        {
            // printf("sending command!\n");
            // Send command to server
            write(client_socket, command, strlen(command));

            // Check for exit command
            // if (strcmp(command, "quit\n") == 0)
            // {
            //     printf("Exiting...\n");
            //     break;
            // }

            if (file_flag)
            {
                printf("recieving file...\n");
                // if (receive_file(client_socket, "temp.tar.gz") == 0)
                // {
                //     printf("file downloaded\n");
                // }
                // else
                // {
                //     printf("error downloading file\n");
                // }
                // int file_fd;
                // char buffer[BUFFER_SIZE];
                // ssize_t read_bytes;
                // // Opening file for writing
                // if ((file_fd = open("temp.tar.gz", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
                // {
                //     perror("open");
                //     exit(EXIT_FAILURE);
                // }

                // // Reading and writing file content
                // while ((read_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
                // {
                //     printf("file...\n");
                //     if (write(file_fd, buffer, read_bytes) == -1)
                //     {
                //         perror("write");
                //         exit(EXIT_FAILURE);
                //     }
                // }

                // // Closing file and socket
                // close(file_fd);
                receive_file(client_socket, unzip);
            }
            else
            {
                // Receive message from server
                memset(server_reply, 0, sizeof(server_reply));
                if (recv(client_socket, server_reply, 2000, 0) < 0)
                {
                    printf("recv failed. Error");
                    break;
                }
                printf("Server reply: %s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }
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
