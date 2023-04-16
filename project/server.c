#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGHT 10000

// commands to be run by the server
// findfiles
// when calling, set initial path to begin with home
void findfile(char *filename, char *path, int *found)
{
    DIR *dir;
    struct dirent *dp;
    struct stat st;
    char buf[BUFFER_SIZE];

    // Open the directory at the specified path
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    // Traverse the directory
    while ((dp = readdir(dir)) != NULL)
    {
        // Check if the current directory entry is the target file
        if (strcmp(dp->d_name, filename) == 0)
        {
            // Construct the full path to the file
            sprintf(buf, "%s/%s", path, filename);
            // Get the file stats
            if (stat(buf, &st) == 0)
            {
                // Print the file information
                printf("File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                // Set the found flag
                *found = 1;
                // Break out of the loop
                break;
            }
        }
        // Recursively search subdirectories
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct the full path to the subdirectory
            sprintf(buf, "%s/%s", path, dp->d_name);
            // Recursively search the subdirectory
            findfile(filename, buf, found);
            // If the file is found, break out of the loop
            if (*found)
            {
                break;
            }
        }
    }

    // Close the directory
    closedir(dir);
}

// sgetfiles
void sgetfiles(char *tar_filename, long size1, long size2)
{
    char complete_files_path[10000];
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];
    // FILE *tar_file = fopen(tar_filename, "wb");
    // if (!tar_file)
    // {
    //     perror("Error creating tar file");
    //     exit(1);
    // }

    DIR *root_dir = opendir(getenv("HOME"));
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    // int file_count = 0;
    while ((dir_entry = readdir(root_dir)) != NULL)
    {
        if (dir_entry->d_type != DT_REG)
        {
            continue;
        }

        sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);

        struct stat file_stats;
        if (stat(file_path, &file_stats) == -1)
        {
            perror("Error getting file stats");
            exit(1);
        }

        if (file_stats.st_size >= size1 && file_stats.st_size <= size2)
        {

            // printf("file path %d: %s\n", file_count, file_path);
            // file_count++;
            sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
            // system("tar -cvf my_archive.tar file1.txt file2.txt");

            // FILE *file = fopen(file_path, "rb");
            // if (!file)
            // {
            //     perror("Error opening file");
            //     exit(1);
            // }

            // char header[100];
            // sprintf(header, "%s\n", dir_entry->d_name);
            // fwrite(header, sizeof(char), strlen(header), tar_file);

            // char size_str[20];
            // sprintf(size_str, "%ld\n", file_stats.st_size);
            // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

            // char buffer[4096];
            // size_t read_bytes;
            // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
            // {
            //     fwrite(buffer, 1, read_bytes, tar_file);
            // }

            // fclose(file);
        }
    }
    sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
    // printf("complete file paths: %s\n", complete_files_path);
    // printf("command: %s\n", command);
    system(command);
    // closedir(root_dir);
    // fclose(tar_file);
}

// dgetfiles
void dgetfiles(char *tar_filename, time_t date1, time_t date2)
{
    char complete_files_path[10000];
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];
    // FILE *tar_file = fopen(tar_filename, "wb");
    // if (!tar_file)
    // {
    //     perror("Error creating tar file");
    //     exit(1);
    // }

    DIR *root_dir = opendir(getenv("HOME"));
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(root_dir)) != NULL)
    {
        if (dir_entry->d_type != DT_REG)
        {
            continue;
        }

        sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
        struct stat file_stats;
        if (stat(file_path, &file_stats) == -1)
        {
            perror("Error getting file stats");
            exit(1);
        }

        if (file_stats.st_mtime >= date1 && file_stats.st_mtime <= date2)
        {
            sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
            // FILE *file = fopen(file_path, "rb");
            // if (!file)
            // {
            //     perror("Error opening file");
            //     exit(1);
            // }

            // char header[100];
            // sprintf(header, "%s\n", dir_entry->d_name);
            // fwrite(header, sizeof(char), strlen(header), tar_file);

            // char size_str[20];
            // sprintf(size_str, "%ld\n", file_stats.st_size);
            // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

            // char buffer[4096];
            // size_t read_bytes;
            // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
            // {
            //     fwrite(buffer, 1, read_bytes, tar_file);
            // }

            // fclose(file);
        }
    }
    sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
    printf("complete file paths: %s\n", complete_files_path);
    printf("command: %s\n", command);
    system(command);
    // closedir(root_dir);
    // fclose(tar_file);
}

// getfiles
void getfiles(char *tar_filename, char **files, int num_files)
{
    char complete_files_path[10000];
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];
    // FILE *tar_file = fopen(tar_filename, "wb");
    // if (!tar_file)
    // {
    //     perror("Error creating tar file");
    //     exit(1);
    // }

    DIR *root_dir = opendir(getenv("HOME"));
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    int found_files = 0;
    for (int i = 0; i < num_files; i++)
    {
        rewinddir(root_dir);
        while ((dir_entry = readdir(root_dir)) != NULL)
        {
            if (dir_entry->d_type != DT_REG)
            {
                continue;
            }

            sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
            if (strcmp(files[i], dir_entry->d_name) == 0)
            {
                found_files++;

                // struct stat file_stats;
                // if (stat(file_path, &file_stats) == -1)
                // {
                //     perror("Error getting file stats");
                //     exit(1);
                // }

                // FILE *file = fopen(file_path, "rb");
                // if (!file)
                // {
                //     perror("Error opening file");
                //     exit(1);
                // }

                sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
                // char header[100];
                // sprintf(header, "%s\n", dir_entry->d_name);
                // fwrite(header, sizeof(char), strlen(header), tar_file);

                // char size_str[20];
                // sprintf(size_str, "%ld\n", file_stats.st_size);
                // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

                // char buffer[4096];
                // size_t read_bytes;
                // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
                // {
                //     fwrite(buffer, 1, read_bytes, tar_file);
                // }

                // fclose(file);
            }
        }
    }

    // closedir(root_dir);
    // fclose(tar_file);

    if (found_files == 0)
    {
        printf("No file found\n");
        exit(0);
    }
    else
    {
        sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
        printf("complete file paths: %s\n", complete_files_path);
        printf("command: %s\n", command);
        system(command);
    }
}
// validity checker for gettarz
int is_valid_extension(char *filename, char **extensions, int num_extensions)
{
    char *dot_pos = strrchr(filename, '.');
    if (!dot_pos)
    {
        return 0;
    }

    char *extension = dot_pos + 1;
    for (int i = 0; i < num_extensions; i++)
    {
        if (strcmp(extension, extensions[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}
// gettarz
void gettarz(char *tar_filename, char **extensions, int num_extensions)
{
    char complete_files_path[10000];
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];
    // FILE *tar_file = fopen(tar_filename, "wb");
    // if (!tar_file)
    // {
    //     perror("Error creating tar file");
    //     exit(1);
    // }

    DIR *root_dir = opendir(getenv("HOME"));
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    int found_files = 0;
    while ((dir_entry = readdir(root_dir)) != NULL)
    {
        if (dir_entry->d_type != DT_REG)
        {
            continue;
        }

        sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
        if (is_valid_extension(dir_entry->d_name, extensions, num_extensions))
        {
            found_files++;
            printf("file: %s\n", dir_entry->d_name);
            sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
            // struct stat file_stats;
            // if (stat(file_path, &file_stats) == -1)
            // {
            //     perror("Error getting file stats");
            //     exit(1);
            // }

            // FILE *file = fopen(file_path, "rb");
            // if (!file)
            // {
            //     perror("Error opening file");
            //     exit(1);
            // }

            // char header[100];
            // sprintf(header, "%s\n", dir_entry->d_name);
            // fwrite(header, sizeof(char), strlen(header), tar_file);

            // char size_str[20];
            // sprintf(size_str, "%ld\n", file_stats.st_size);
            // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

            // char buffer[4096];
            // size_t read_bytes;
            // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
            // {
            //     fwrite(buffer, 1, read_bytes, tar_file);
            // }

            // fclose(file);
        }
    }

    // closedir(root_dir);
    // fclose(tar_file);

    if (found_files == 0)
    {
        printf("No file found\n");
        exit(0);
    }
    else
    {

        sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
        printf("complete file paths: %s\n", complete_files_path);
        printf("command: %s\n", command);
        system(command);
    }
}

// commands end here

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