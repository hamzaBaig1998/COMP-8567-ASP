#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void process_client(int client_socket)
{
    char command[1024];
    int n;

    while (1)
    {
        // Receive command from client
        n = recv(client_socket, command, sizeof(command), 0);
        if (n <= 0)
        {
            break;
        }

        // Parse command and perform action
        // ...

        // Send response to client
        char response[] = "Response from server";
        send(client_socket, response, strlen(response), 0);

        // Check for quit command
        if (strcmp(command, "quit\n") == 0)
        {
            break;
        }
    }

    // Close client socket
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    pid_t pid;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8000);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Listening for connections\n");
    // Server loop
    while (1)
    {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        // Fork child process to handle client request
        pid = fork();
        if (pid == -1)
        {
            perror("Error forking child process");
            close(client_socket);
            continue;
        }
        else if (pid == 0)
        {
            // Child process
            process_client(client_socket);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(client_socket);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}