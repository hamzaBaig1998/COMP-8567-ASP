#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int server_fd, client_fd, file_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setting socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Setting server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for client on port %d\n", PORT);

    // Accepting incoming connection
    client_addr_len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Opening tar file
    if ((file_fd = open("temp.tar.gz", O_RDONLY)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Reading and sending file content
    ssize_t read_bytes;
    while ((read_bytes = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (send(client_fd, buffer, read_bytes, 0) == -1)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    // Closing file and sockets
    close(file_fd);
    close(client_fd);
    close(server_fd);

    printf("File transfer completed\n");

    return 0;
}