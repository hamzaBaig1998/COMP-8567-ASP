#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 12 // Maximum number of clients (including server and mirror)
#define BUFFER_SIZE 1024 // Buffer size for receiving data from clients

void handle_client(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    int clients_count = 0;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error: Failed to create socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Port number for server

    // Bind the server address to the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error: Failed to bind socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Error: Failed to listen");
        exit(1);
    }

    printf("Server is listening for incoming connections...\n");

    while (1) {
        // Accept incoming connections
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Error: Failed to accept connection");
            exit(1);
        }

        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle the first 4 client connections in the server
        if (clients_count < 4) {
            handle_client(client_socket);
            clients_count++;
        }
        // Handle the next 4 client connections in the mirror
        else if (clients_count >= 4 && clients_count < 8) {
            printf("Client %d handled by the mirror\n", clients_count + 1);
            clients_count++;
        }
        // Handle the remaining client connections alternately between server and mirror
        else {
            if (clients_count % 2 == 0) {
                handle_client(client_socket);
            } else {
                printf("Client %d handled by the mirror\n", clients_count + 1);
            }
            clients_count++;
        }
    }

    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Error: Failed to receive data from client");
        exit(1);
    }

    printf("Received data from client: %s\n", buffer);

    // Send data back to client
    if (send(client_socket, buffer, bytes_received, 0) == -1) {
        perror("Error: Failed to send data to client");
        exit(1);
    }

    // Close the client socket
    close(client_socket);
}
