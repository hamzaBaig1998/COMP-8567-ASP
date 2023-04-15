#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int client_socket;
    struct sockaddr_in server_address;
    char command[1024];
    int n;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8000);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Get command from user
        printf("> ");
        fgets(command, sizeof(command), stdin);

        // Verify syntax of command
        // ...

        // Send command to server
        n = send(client_socket, command, strlen(command), 0);
        if (n == -1)
        {
            perror("Error sending command to server");
            continue;
        }

        // Receive response from server
        char response[1024];
        n = recv(client_socket, response, sizeof(response), 0);
        if (n == -1)
        {
            perror("Error receiving response from server");
            continue;
        }
        else if (n == 0)
        {
            printf("Server closed connection\n");
            break;
        }

        // Print response to terminal
        printf("%s", response);

        // Check for -u option
        if (strstr(command, "-u"))
        {
            // Receive tar.gz file from server and extract it
            // ...
        }

        // Check for quit command
        if (strcmp(command, "quit\n") == 0)
        {
            break;
        }
    }

    // Close socket
    close(client_socket);

    return 0;
}