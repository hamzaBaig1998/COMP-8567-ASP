#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 8081 // Port number of the mirror server
#define BUFFER_SIZE 1024 // Buffer size for receiving data from clients


void processclient(int client_socket);

void processclient(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        // Wait for client to send a command
        int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            //perror("Error: No bytes received from client mirror");
            break; // Connection closed or error
        }
        // Process command
        // Example: echo back the received command
        printf("Client command received from mirror: %s\n",buffer);
        send(client_socket, buffer, bytes_read, 0);
        bzero(buffer, BUFFER_SIZE);
    }
    close(client_socket);
}

int main() {
    int mirror_socket, client_socket, opt = 1;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received, addrlen = sizeof(client_addr);
    int clients_count=0;

    // Create a socket
    mirror_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mirror_socket == -1) {
        perror("Error: Failed to create mirror socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Set socket options to reuse address
    if (setsockopt(mirror_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error: Failed to set socket options");
        exit(1);
    }

    // Bind the socket to the server address
    if (bind(mirror_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error: Failed to bind mirror socket to server address");
        exit(1);
    }

    // Listen for incoming client connections
    if (listen(mirror_socket, 4) == -1) {
        perror("Error: Failed to listen for incoming connections");
        exit(1);
    }

    printf("Mirror server listening on %s:%d\n", SERVER_IP, SERVER_PORT);

    // Accept client connections and mirror incoming data to standard output
    while (1) {
        // Accept incoming client connections
        client_socket = accept(mirror_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
        if (client_socket == -1) {
            perror("Error: Failed to accept incoming client connection");
            continue;
        }

        printf("Mirror server accepted incoming client connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive data from the client and mirror it to the standard output
        /*while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            printf("Mirror server received %d bytes from client: %s\n", bytes_received, buffer);
            fflush(stdout); // Flush standard output buffer
            if (send(client_socket, buffer, bytes_received, 0) == -1) {
                perror("Error: Failed to send mirrored data to client");
                break;
            }
        }*/

        pid_t pid = fork();
        if (pid == -1) {
            close(client_socket);
            perror("Error: Coult not fork to handle client");
            continue;
        } else if (pid == 0) {
        // Child process
        printf("Client %d handled by the mirror\n", clients_count + 1);
        close(mirror_socket); // Close unused server socket
        processclient(client_socket);
        exit(EXIT_SUCCESS);
        } else {
        clients_count++; //Increment count of clients.
        printf("No. of Clients handled: %d\n", clients_count);
        // Parent process
        close(client_socket); // Close unused client socket
        
        }


        // Close the client socket
        //close(client_socket);

        printf("Mirror server closed client connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    // Close the mirror server socket
    close(mirror_socket);

    return 0;
}





