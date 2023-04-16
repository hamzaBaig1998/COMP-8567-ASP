#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define SERVER_IP "127.0.0.1"
#define MAX_CLIENTS 12 // Maximum number of clients (including server and mirror)
#define BUFFER_SIZE 1024 // Buffer size for receiving data from clients
 FILE *fp;
  char fileBuffer[1024] = {0};

void processclient(int client_socket);
void handle_mirror(int client_socket);

void processclient(int client_socket) {

     fp = fopen("teat.zip", "rb");
    if (fp == NULL) {
        printf("File not found.\n");
        //break;
    }

    char buffer[BUFFER_SIZE] = "In processclient()";
    send(client_socket, buffer, 18,0);
    bzero(buffer, BUFFER_SIZE);
    //wait for for client
    while (1) {
        // int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        // if (bytes_read <= 0) {
        //     perror("Error: No bytes received from client");
        //     break; // Connection closed or error
        // }
        // Process command
        // Example: echo back the received command
        printf("Client command received from server: %s\n",buffer);
        //sending commend to client
        size_t bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), fp);
        if (bytesRead == 0) {
            break;
        }
        send(client_socket, fileBuffer, bytesRead, 0);
        bzero(buffer, BUFFER_SIZE);
    }
    close(client_socket);
    fclose(fp);
}
/*
void handle_mirror(int client_socket) {
    char buffer[22] = "Mirror: 127.0.0.1 8081";// Local IP
    //char buffer[25] = "Mirror: 192.168.2.16 8081";// Router IP
    //Send the mirror address to client for mirror connection
    send(client_socket, buffer, 22, 0);// 22 for local 25 for router
    close(client_socket);
}
*/

void handle_mirror(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        //perror("Error: Failed to receive data from client");
        exit(1);
    }

    printf("Received data from client: %s\n", buffer);

    // Connect to mirror server
    int mirror_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mirror_socket == -1) {
        perror("Error: Failed to create mirror socket");
        exit(1);
    }

    struct sockaddr_in mirror_addr;
    mirror_addr.sin_family = AF_INET;
    mirror_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    mirror_addr.sin_port = htons(8081);

    if (connect(mirror_socket, (struct sockaddr*)&mirror_addr, sizeof(mirror_addr)) == -1) {
        perror("Error: Failed to connect to mirror server");
        exit(1);
    }

    // Send data to mirror server
    if (send(mirror_socket, buffer, bytes_received, 0) == -1) {
        perror("Error: Failed to send data to mirror server");
        exit(1);
    }

    // Receive data from mirror server
    bytes_received = recv(mirror_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Error: Failed to receive data from mirror server");
        exit(1);
    }

    printf("Received data from mirror server: %s\n", buffer);

    // Send data back to client
    if (send(client_socket, buffer, bytes_received, 0) == -1) {
        perror("Error: Failed to send data to client");
        exit(1);
    }

    // Close sockets
    close(mirror_socket);
    close(client_socket);
}


int main() {
      
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    int clients_count = 0;
    int opt=1;
    

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error: Failed to create socket");
        exit(1);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Error: Failed to set 'setsockopt' for reusing address");
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);//INADDR_ANY;
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

        printf("\nClient connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle the first 4 client connections in the server
        if (clients_count < 4 || (clients_count >= 8 && clients_count%2 == 1)) {
            //handle_client(client_socket);
            //clients_count++;
            pid_t pid = fork();
            if (pid == -1) {
                close(client_socket);
                perror("Error: Coult not fork to handle client");
                continue;
            } else if (pid == 0) {
                // Child process
                 printf("Client %d handled by the servor\n", clients_count + 1);
                close(server_socket); // Close unused server socket
                processclient(client_socket);
                exit(EXIT_SUCCESS);
            } else {
                clients_count++; //Increment count of clients.
                printf("No. of Clients handled: %d\n", clients_count);
                // Parent process
                close(client_socket); // Close unused client socket
                printf("server closed client connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
        }
        
        // Handle the next 4 client connections in the mirror
        //else if (clients_count >= 4 && clients_count < 8) {
        else{
           
            //clients_count++;
            pid_t pid = fork();
            if (pid == -1) {
                close(client_socket);
                perror("Error: Coult not fork to handle client");
                continue;
            } else if (pid == 0) {
                // Child process
                printf("Client %d handled by the mirror\n", clients_count + 1);
                close(server_socket); // Close unused server socket
                handle_mirror(client_socket);
                exit(EXIT_SUCCESS);
            } else {
                clients_count++; //Increment count of clients.
                printf("No. of Clients handled: %d\n", clients_count);
                // Parent process
                close(client_socket); // Close unused client socket
                printf("Mirror server(server.c) closed client connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
        }
        /*
        // Handle the remaining client connections alternately between server and mirror
        else {
            if (clients_count % 2 == 0) {
                handle_client(client_socket);
            } else {
                printf("Client %d handled by the mirror\n", clients_count + 1);
            }
            clients_count++;
        }
        */
    }

    return 0;
}
/*
void processclient(int client_socket) {
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
*/