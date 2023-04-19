// github: https://github.com/hamzaBaig1998/COMP-8567-ASP/tree/main/project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define O_BINARY 0

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 8081      // Port number of the server
#define BUFFER_SIZE 1024      // Buffer size for sending data to server
#define MAX_PATH_LENGTH 4096

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

    // Send data to the server
    sprintf(buffer, "Hello from client");
    if (send(client_socket, buffer, strlen(buffer), 0) == -1)
    {
        perror("Error: Failed to send data to server");
        exit(1);
    }

    // Receive data from the server
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1)
    {
        perror("Error: Failed to receive data from server");
        exit(1);
    }

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        if (write(fp, buffer, bytes_received) != bytes_received)
        {
            perror("Error writing to file");
            exit(1);
        }
    }

    printf("Received data from server: %s\n", buffer);

    // Close the client socket
    close(client_socket);

    return 0;
}
