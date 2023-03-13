#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int socket_fd, read_size;
    struct sockaddr_in server;
    char *message, server_reply[2000];

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        printf("Could not create socket");
        return 1;
    }

    // Prepare sockaddr_in structure
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Loopback IP address
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Connect to server
    if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Connected\n");

    // Receive message from server
    if ((read_size = recv(socket_fd, server_reply, 2000, 0)) > 0)
    {
        printf("%s", server_reply);
    }

    int flag = 1;
    while (flag)
    {
        printf("Client:");
        fgets(message, sizeof(message), stdin); // read string

        // if (strcmp(message, "exit"))
        // {
        //     flag = 0;
        //     return 0;
        // }
        // Send message to server
        // message = "Hello Server, I am the client\n";
        write(socket_fd, message, strlen(message));

        // Receive message from server
        while ((read_size = recv(socket_fd, server_reply, 2000, 0)) > 0)
        {
            printf("%s", server_reply);
        }
    }

    return 0;
}
