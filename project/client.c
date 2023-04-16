#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_address;

    // Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address
    server_address.sin_port = htons(12345);                  // Replace with the server port number
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send a message to the server
    char message[] = "Hello, server!";
    if (send(client_socket, message, strlen(message), 0) == -1)
    {
        perror("send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive a response from the server
    char response[1024];
    int bytes_received = recv(client_socket, response, sizeof(response), 0);
    if (bytes_received == -1)
    {
        if (errno == ECONNRESET)
        {
            printf("Connection reset by peer\n");
        }
        else
        {
            perror("recv");
        }
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    else if (bytes_received == 0)
    {
        printf("Connection closed by peer\n");
        close(client_socket);
        exit(EXIT_SUCCESS);
    }
    response[bytes_received] = '\0';
    printf("Server response: %s\n", response);

    // Close the socket
    close(client_socket);

    return 0;
}