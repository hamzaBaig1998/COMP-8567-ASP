// Note: This code only handles the server part of the communication,
// and does not implement the logic for the commands listed in Section 2.
// The processclient() function needs to be further implemented to handle the
// specific commands as required.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define MAX_CLIENTS 4
#define BUFFER_SIZE 256
int CLIENT_NO = 0;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void quit_handler(int sig)
{
    CLIENT_NO--; // Decrement count of clients
    printf("No. of Clients Connected: %d\n", CLIENT_NO);
}

void processclient(int client_socket)
{
    char buffer[BUFFER_SIZE] = {0};
    while (1)
    {
        // Wait for client to send a command
        int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0)
        {
            break; // Connection closed or error
        }
        // Process command
        // Example: echo back the received command
        printf("Got client command: %s\n", buffer);
        send(client_socket, buffer, bytes_read, 0);
        bzero(buffer, BUFFER_SIZE);
    }
    close(client_socket);
}

int main(int argc, char const *argv[])
{
    int portno;
    int opt = 1;
    signal(SIGCHLD, quit_handler);
    // create server socket similar to what was done in client
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0)
    {
        error("Could not create socket");
    }
    // Set socket options to reuse address and port
    if (setsockopt(servSockD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        error("setsockopt failed");
    }
    struct sockaddr_in servAddr, cliAddr;
    // define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    if (argc != 2)
    {
        printf("Usage: ./server portno");
        exit(1);
    }
    sscanf(argv[1], "%d", &portno);
    servAddr.sin_port = htons((uint16_t)portno); // Host to network short- byte order of the port number

    // bind socket to the specified IP and port
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        error("Bind failed");
    }
    printf("Listening on %d\n", portno);

    // listen for connections
    if (listen(servSockD, 5) < 0)
    { // Max 5 connections in buffer.
        error("listen failed");
    }
    int client_socket;

    while (1)
    {
        // Accept incoming connection
        if ((client_socket = accept(servSockD, (struct sockaddr *)&cliAddr, (socklen_t *)&cliAddr)) < 0)
        {
            perror("accept failed");
            continue;
        }

        // Fork child process to handle client request
        pid_t pid = fork();
        if (pid == -1)
        {
            close(client_socket);
            perror("fork failed");
            continue;
        }
        else if (pid == 0)
        {
            // Child process
            close(servSockD); // Close unused server socket
            processclient(client_socket);
            exit(EXIT_SUCCESS);
        }
        else
        {
            CLIENT_NO++; // Increment count of clients.
            printf("No. of Clients Connected: %d\n", CLIENT_NO);
            // Parent process
            close(client_socket); // Close unused client socket
        }
    }
    close(servSockD);
    return 0;
}
