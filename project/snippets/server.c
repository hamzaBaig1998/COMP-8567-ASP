#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int socket_fd, client_fd, read_size;
    struct sockaddr_in server, client;
    char *message, client_message[2000];

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        printf("Could not create socket");
        return 1;
    }

    // Prepare sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind socket to port
    if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    printf("Socket binded\n");

    // Listen for incoming connections
    listen(socket_fd, 3);

    // Accept incoming connection
    printf("Waiting for incoming connections...\n");
    int c = sizeof(struct sockaddr_in);
    client_fd = accept(socket_fd, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_fd < 0)
    {
        perror("accept failed");
        return 1;
    }
    printf("Connection accepted\n");
    // Send message to client
    message = "Hello Client, I have received your connection\n";
    write(client_fd, message, strlen(message));

    while (1)
    {
        // Send message to client
        printf("Server:");
        fgets(message, sizeof(message), stdin); // read string
        write(client_fd, message, strlen(message));

        // Receive message from client
        while ((read_size = recv(client_fd, client_message, 2000, 0)) > 0)
        {
            // Send message back to client
            write(client_fd, client_message, strlen(client_message));
        }
        if (read_size == 0)
        {
            printf("Client disconnected\n");
        }
        else if (read_size == -1)
        {
            perror("recv failed");
        }
    }
    return 0;
}
