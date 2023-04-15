#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

void handle_client(int client_socket)
{
    char buffer[1024];
    ssize_t bytes_received;

    // Receive data from the client
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
    {
        perror("recv");
        close(client_socket);
        return;
    }

    // Process the data
    printf("Received %ld bytes (server): %.*s\n", bytes_received, (int)bytes_received, buffer);

    // Send a message back to the client
    const char *message = "Hello, client!";
    ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
    if (bytes_sent == -1)
    {
        perror("send");
    }

    // Close the client socket
    close(client_socket);
}

void handle_mirror(int client_socket)
{
    char buffer[22] = "Mirror: 127.0.0.1 8081"; // Local IP
    // char buffer[25] = "Mirror: 192.168.2.16 8081";// Router IP
    // Send the mirror address to client for mirror connection
    printf("client socket: %d", client_socket);
    send(client_socket, buffer, 22, 0); // 22 for local 25 for router
    close(client_socket);
}

void start_mirror_process()
{
    printf("Mirror Process started\n");
    pid_t mirror_pid = fork();
    if (mirror_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (mirror_pid == 0)
    {
        if (access("./mirror", F_OK) == -1)
        {
            perror("access");
            exit(EXIT_FAILURE);
        }
        execlp("./mirror", "./mirror", NULL); // Start the mirror process
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_count = 0;

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening for clients\n");

    while (1)
    {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == -1)
        {
            perror("accept");
            continue;
        }

        // Handle in server for first 4
        if (client_count < 4)
        {
            // Create a child process to handle the client connection
            pid_t child_pid = fork();
            if (child_pid == -1)
            {
                perror("fork");
                close(client_socket);
                continue;
            }
            else if (child_pid == 0)
            {
                close(server_socket);
                handle_client(client_socket);
                exit(EXIT_SUCCESS);
            }
            else
            {
                close(client_socket);
                client_count++;
                continue;
            }
        }

        // Check if it's time to start the mirror process
        else if (client_count >= 4 && client_count < 8)
        {
            // start_mirror_process();
            handle_mirror(client_socket);
            client_count++;
        }
        else
        {
            if (client_count % 2 == 0)
            {
                // Create a child process to handle the client connection
                pid_t child_pid = fork();
                if (child_pid == -1)
                {
                    perror("fork");
                    close(client_socket);
                    continue;
                }
                else if (child_pid == 0)
                {
                    close(server_socket);
                    handle_client(client_socket);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    close(client_socket);
                    client_count++;
                    continue;
                }
            }
            else
            {
                // start_mirror_process();
                handle_mirror(client_socket);
            }
            client_count++;
        }
    }

    // Close the server socket
    close(server_socket);

    return 0;
}