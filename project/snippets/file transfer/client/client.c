#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int client_fd, file_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t read_bytes;

    // Creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setting server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == -1)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connecting to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // Opening file for writing
    if ((file_fd = open("temp.tar.gz", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Reading and writing file content
    while ((read_bytes = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        if (write(file_fd, buffer, read_bytes) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Closing file and socket
    close(file_fd);
    close(client_fd);

    printf("File transfer completed\n");

    return 0;
}