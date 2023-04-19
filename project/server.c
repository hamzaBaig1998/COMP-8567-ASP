// github: https://github.com/hamzaBaig1998/COMP-8567-ASP/tree/main/project
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
#include <fcntl.h>

#define SERVER_IP "127.0.0.1"
#define MAX_CLIENTS 12   // Maximum number of clients (including server and mirror)
#define BUFFER_SIZE 4096 // Buffer size for receiving data from clients

#define SERVER_PORT 8082
#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGHT 10000
#define MAX_ARGS 10
#define MAX_CMD_LEN 2048

FILE *fp;
char fileBuffer[1024] = {0};

void processclient(int client_socket);
void handle_mirror(int client_socket);

// commands to be run by the server
// findfiles
// when calling, set initial path to begin with home
void findfile(int client_socket, char *filename, char *path, int *found)
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
                // sprintf(*message, "File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                // printf("message from fucntion:%s", *message);
                printf("File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                char message[1000];
                memset(message, 0, sizeof(message));
                sprintf(message, "File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                message[strlen(message)] = '\0';
                if (send(client_socket, message, strlen(message), 0) < 0)
                {
                    perror("send failed");
                    exit(EXIT_FAILURE);
                }
                memset(message, 0, sizeof(message));
                // Set the found flag
                *found = 1;
                // return message;
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
            findfile(client_socket, filename, buf, found);
            // If the file is found, break out of the loop
            if (*found)
            {
                // sprintf(*message, "file not found");
                // printf("file not found");
                // char message[1000];
                // sprintf(message, "file not found");
                // write(client_socket, message, strlen(message));
                //  return message;
                break;
            }
        }
    }
    // Close the directory
    closedir(dir);
}

// sgetfiles
// void sgetfiles(char *tar_filename, long size1, long size2)
// {
//     char complete_files_path[10000];
//     char command[MAX_COMMAND_LENGHT];
//     char file_path[MAX_PATH_LENGTH];
//     // FILE *tar_file = fopen(tar_filename, "wb");
//     // if (!tar_file)
//     // {
//     //     perror("Error creating tar file");
//     //     exit(1);
//     // }

//     DIR *root_dir = opendir(getenv("HOME"));
//     if (!root_dir)
//     {
//         perror("Error opening root directory");
//         exit(1);
//     }

//     struct dirent *dir_entry;
//     // int file_count = 0;
//     while ((dir_entry = readdir(root_dir)) != NULL)
//     {
//         if (dir_entry->d_type != DT_REG)
//         {
//             continue;
//         }

//         sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);

//         struct stat file_stats;
//         if (stat(file_path, &file_stats) == -1)
//         {
//             perror("Error getting file stats");
//             exit(1);
//         }

//         if (file_stats.st_size >= size1 && file_stats.st_size <= size2)
//         {

//             // printf("file path %d: %s\n", file_count, file_path);
//             // file_count++;
//             sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
//             // system("tar -cvf my_archive.tar file1.txt file2.txt");

//             // FILE *file = fopen(file_path, "rb");
//             // if (!file)
//             // {
//             //     perror("Error opening file");
//             //     exit(1);
//             // }

//             // char header[100];
//             // sprintf(header, "%s\n", dir_entry->d_name);
//             // fwrite(header, sizeof(char), strlen(header), tar_file);

//             // char size_str[20];
//             // sprintf(size_str, "%ld\n", file_stats.st_size);
//             // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

//             // char buffer[4096];
//             // size_t read_bytes;
//             // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
//             // {
//             //     fwrite(buffer, 1, read_bytes, tar_file);
//             // }

//             // fclose(file);
//         }
//     }
//     sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
//     // printf("complete file paths: %s\n", complete_files_path);
//     // printf("command: %s\n", command);
//     system(command);
//     // closedir(root_dir);
//     // fclose(tar_file);
// }

void sgetfiles(const char *dir_path, const char *tar_path, const char *size1, const char *size2)
{
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, MAX_CMD_LEN, "find %s -type f -size +%s -size -%s -print0 | xargs -0 tar -czf %s", dir_path, size1, size2, tar_path);
    system(cmd);
}
// dgetfiles
// void dgetfiles(char *tar_filename, time_t date1, time_t date2)
// {
//     char complete_files_path[10000];
//     char command[MAX_COMMAND_LENGHT];
//     char file_path[MAX_PATH_LENGTH];
//     // FILE *tar_file = fopen(tar_filename, "wb");
//     // if (!tar_file)
//     // {
//     //     perror("Error creating tar file");
//     //     exit(1);
//     // }

//     DIR *root_dir = opendir(getenv("HOME"));
//     if (!root_dir)
//     {
//         perror("Error opening root directory");
//         exit(1);
//     }

//     struct dirent *dir_entry;
//     while ((dir_entry = readdir(root_dir)) != NULL)
//     {
//         if (dir_entry->d_type != DT_REG)
//         {
//             continue;
//         }

//         sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
//         struct stat file_stats;
//         if (stat(file_path, &file_stats) == -1)
//         {
//             perror("Error getting file stats");
//             exit(1);
//         }

//         if (file_stats.st_mtime >= date1 && file_stats.st_mtime <= date2)
//         {
//             sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
//             // FILE *file = fopen(file_path, "rb");
//             // if (!file)
//             // {
//             //     perror("Error opening file");
//             //     exit(1);
//             // }

//             // char header[100];
//             // sprintf(header, "%s\n", dir_entry->d_name);
//             // fwrite(header, sizeof(char), strlen(header), tar_file);

//             // char size_str[20];
//             // sprintf(size_str, "%ld\n", file_stats.st_size);
//             // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

//             // char buffer[4096];
//             // size_t read_bytes;
//             // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
//             // {
//             //     fwrite(buffer, 1, read_bytes, tar_file);
//             // }

//             // fclose(file);
//         }
//     }
//     sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
//     printf("complete file paths: %s\n", complete_files_path);
//     printf("command: %s\n", command);
//     system(command);
//     // closedir(root_dir);
//     // fclose(tar_file);
// }
void dgetfiles(const char *dir_path, const char *tar_path, const char *date1, const char *date2)
{
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, MAX_CMD_LEN, "find %s -type f -newermt '%s' ! -newermt '%s' -print0 | xargs -0 tar -czf %s", dir_path, date1, date2, tar_path);
    system(cmd);
}
// getfiles
void search_directory(char *dir_path, char **files, int num_files, char *complete_files_path)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        perror("Error opening directory");
        exit(1);
    }

    struct dirent *entry;
    char file_path[MAX_PATH_LENGTH];
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            sprintf(file_path, "%s/%s", dir_path, entry->d_name);
            search_directory(file_path, files, num_files, complete_files_path);
        }
        else if (entry->d_type == DT_REG)
        {
            for (int i = 0; i < num_files; i++)
            {
                if (strcmp(files[i], entry->d_name) == 0)
                {
                    char file_path[MAX_PATH_LENGTH];
                    sprintf(file_path, "%s/%s", dir_path, entry->d_name);
                    sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
                }
            }
        }
    }

    closedir(dir);
}

void getfiles(char *tar_filename, char **files, int num_files, int found)
{
    char complete_files_path[MAX_COMMAND_LENGHT] = ""; // Initialize to empty string
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];

    search_directory(getenv("HOME"), files, num_files, complete_files_path);

    if (strlen(complete_files_path) == 0)
    {
        found = 0;
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
    // char complete_files_path[10000];
    // char command[MAX_COMMAND_LENGHT];
    // char file_path[MAX_PATH_LENGTH];
    // // FILE *tar_file = fopen(tar_filename, "wb");
    // // if (!tar_file)
    // // {
    // //     perror("Error creating tar file");
    // //     exit(1);
    // // }

    // DIR *root_dir = opendir(getenv("HOME"));
    // if (!root_dir)
    // {
    //     perror("Error opening root directory");
    //     exit(1);
    // }

    // struct dirent *dir_entry;
    // int found_files = 0;
    // for (int i = 0; i < num_files; i++)
    // {
    //     rewinddir(root_dir);
    //     while ((dir_entry = readdir(root_dir)) != NULL)
    //     {
    //         if (dir_entry->d_type != DT_REG)
    //         {
    //             continue;
    //         }

    //         sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
    //         if (strcmp(files[i], dir_entry->d_name) == 0)
    //         {
    //             found_files++;

    //             // struct stat file_stats;
    //             // if (stat(file_path, &file_stats) == -1)
    //             // {
    //             //     perror("Error getting file stats");
    //             //     exit(1);
    //             // }

    //             // FILE *file = fopen(file_path, "rb");
    //             // if (!file)
    //             // {
    //             //     perror("Error opening file");
    //             //     exit(1);
    //             // }

    //             sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
    //             // char header[100];
    //             // sprintf(header, "%s\n", dir_entry->d_name);
    //             // fwrite(header, sizeof(char), strlen(header), tar_file);

    //             // char size_str[20];
    //             // sprintf(size_str, "%ld\n", file_stats.st_size);
    //             // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

    //             // char buffer[4096];
    //             // size_t read_bytes;
    //             // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
    //             // {
    //             //     fwrite(buffer, 1, read_bytes, tar_file);
    //             // }

    //             // fclose(file);
    //         }
    //     }
    // }

    // // closedir(root_dir);
    // // fclose(tar_file);

    // if (found_files == 0)
    // {
    //     printf("No file found\n");
    //     exit(0);
    // }
    // else
    // {
    //     sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
    //     printf("complete file paths: %s\n", complete_files_path);
    //     printf("command: %s\n", command);
    //     system(command);
    // }
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

void search_and_add_files(char *root_path, char *tar_filename, char **extensions, int num_extensions, char *complete_files_path)
{
    DIR *root_dir = opendir(root_path);
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(root_dir)) != NULL)
    {
        char file_path[MAX_PATH_LENGTH];
        sprintf(file_path, "%s/%s", root_path, dir_entry->d_name);

        if (dir_entry->d_type == DT_DIR && strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
        {
            search_and_add_files(file_path, tar_filename, extensions, num_extensions, complete_files_path);
        }
        else if (dir_entry->d_type == DT_REG && is_valid_extension(dir_entry->d_name, extensions, num_extensions))
        {
            printf("file: %s\n", file_path);
            sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
        }
    }

    closedir(root_dir);
}

// gettarz
void gettargz(char *tar_filename, char **extensions, int num_extensions)
{
    char complete_files_path[10000] = "";
    char command[MAX_COMMAND_LENGHT] = "";

    search_and_add_files(getenv("HOME"), tar_filename, extensions, num_extensions, complete_files_path);

    if (strlen(complete_files_path) == 0)
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
    // char complete_files_path[10000];
    // char command[MAX_COMMAND_LENGHT];
    // char file_path[MAX_PATH_LENGTH];
    // // FILE *tar_file = fopen(tar_filename, "wb");
    // // if (!tar_file)
    // // {
    // //     perror("Error creating tar file");
    // //     exit(1);
    // // }

    // DIR *root_dir = opendir(getenv("HOME"));
    // if (!root_dir)
    // {
    //     perror("Error opening root directory");
    //     exit(1);
    // }

    // struct dirent *dir_entry;
    // int found_files = 0;
    // while ((dir_entry = readdir(root_dir)) != NULL)
    // {
    //     if (dir_entry->d_type != DT_REG)
    //     {
    //         continue;
    //     }

    //     sprintf(file_path, "%s/%s", getenv("HOME"), dir_entry->d_name);
    //     if (is_valid_extension(dir_entry->d_name, extensions, num_extensions))
    //     {
    //         found_files++;
    //         printf("file: %s\n", dir_entry->d_name);
    //         sprintf(complete_files_path, "%s%s%s", complete_files_path, file_path, " ");
    //         // struct stat file_stats;
    //         // if (stat(file_path, &file_stats) == -1)
    //         // {
    //         //     perror("Error getting file stats");
    //         //     exit(1);
    //         // }

    //         // FILE *file = fopen(file_path, "rb");
    //         // if (!file)
    //         // {
    //         //     perror("Error opening file");
    //         //     exit(1);
    //         // }

    //         // char header[100];
    //         // sprintf(header, "%s\n", dir_entry->d_name);
    //         // fwrite(header, sizeof(char), strlen(header), tar_file);

    //         // char size_str[20];
    //         // sprintf(size_str, "%ld\n", file_stats.st_size);
    //         // fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

    //         // char buffer[4096];
    //         // size_t read_bytes;
    //         // while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
    //         // {
    //         //     fwrite(buffer, 1, read_bytes, tar_file);
    //         // }

    //         // fclose(file);
    //     }
    // }

    // // closedir(root_dir);
    // // fclose(tar_file);

    // if (found_files == 0)
    // {
    //     printf("No file found\n");
    //     exit(0);
    // }
    // else
    // {

    //     sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", complete_files_path);
    //     printf("complete file paths: %s\n", complete_files_path);
    //     printf("command: %s\n", command);
    //     system(command);
    // }
}

// commands end here

// commandshandler
// break commands
// void breakCommands(char *output[50], char *rwCmd)
// {
//     int len = 0;
//     char *ptr = strtok(rwCmd, " ");
//     while (1)
//     {
//         if (ptr == NULL)
//         {
//             output[len] = NULL;
//             len++;
//             break;
//         }
//         if (strcmp(ptr, "\n") == 0)
//         {
//             continue;
//         }
//         output[len] = ptr;
//         len++;
//         ptr = strtok(NULL, " ");
//     }

//     if (len < 1 || len > 9)
//     {
//         printf("output arguments should be greater than equal to 1 and less than equal to 8\n");
//         exit(10);
//     }
// }
// int split_string(char *str, char **words)
// {
//     int word_count = 0;
//     char *token = strtok(str, " ");
//     while (token != NULL && word_count < MAX_WORDS)
//     {
//         words[word_count] = malloc(strlen(token) + 1);
//         strcpy(words[word_count], token);
//         word_count++;
//         token = strtok(NULL, " ");
//     }
//     return word_count;
// }

// command recieved handler
// void commandHandle(char *message, char *result)
// {
//     char *command[10];
//     split_string(command, message);

//     if (strcmp(command[0], "findfile") == 0)
//     {

//         findfile();
//         return;
//     }

//     if (strcmp(command[0], "sgetfiles") == 0)
//     {
//         sgetfiles();
//         return;
//     }

//     if (strcmp(command[0], "dgetfiles") == 0)
//     {
//         dgetfiles();
//         return;
//     }

//     if (strcmp(command[0], "getfiles") == 0)
//     {
//         getfiles();
//         return;
//     }

//     if (strcmp(command[0], "gettargz") == 0)
//     {
//         gettarz();
//         return;
//     }

//     if (strcmp(command[0], "quit") == 0)
//     {
//         strcpy(result, "Exit");
//         return;
//     }
//     strcpy(result, "Bad Request!, Command not supported by the server\n");
// }
// send file to client
int send_file(int socketFd, char *fp)
{
    int pid = fork();
    if (pid == 0)
    {
        int filefd, len;
        fprintf(stderr, "Copying file to client %d\n", socketFd);
        filefd = open(fp, O_RDONLY);
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        while ((len = read(filefd, buffer, BUFFER_SIZE)) > 0)
        {
            if (send(socketFd, buffer, len, 0) == -1)
            {
                perror("send");
                return -1;
            }
        }
        memset(buffer, 0, sizeof(buffer));
        sleep(1);
        fprintf(stderr, "File transfer to client successfully\n");
        close(filefd);
        close(socketFd);
        exit(0);
    }

    wait(NULL);
    return 1;
}

void processclient(int client_socket)
{

    // fp = fopen("teat.zip", "rb");
    // if (fp == NULL)
    // {
    //     printf("File not found.\n");
    //     // break;
    // }

    char buffer[BUFFER_SIZE];
    int file_fd;
    // send(client_socket, buffer, 18, 0);
    // bzero(buffer, BUFFER_SIZE);
    // wait for for client
    // while (1)
    // {
    //     int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
    //     printf("Bytes: %d", bytes_read);
    //     if (bytes_read <= 0)
    //     {
    //         perror("Error: No bytes received from client");
    //         break; // Connection closed or error
    //     }
    //     printf("Bytes: %d", bytes_read);
    //     if (send(client_socket, buffer, sizeof(buffer), 0) == -1)
    //     {
    //         perror("Error: Failed to send data to client");
    //         exit(1);
    //     }
    //     // send(client_socket, fileBuffer, bytesRead, 0);
    //     // Process command
    //     // Example: echo back the received command
    //     // printf("Client command received from server: %s\n", buffer);
    //     // // sending commend to client
    //     // size_t bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), fp);
    //     // if (bytesRead == 0)
    //     // {
    //     //     break;
    //     // }
    //     // send(client_socket, fileBuffer, bytesRead, 0);
    //     // bzero(buffer, BUFFER_SIZE);
    // }
    // Loop to receive and send messages
    char client_message[2000];
    char command[1000], server_reply[2000];
    char *args[MAX_ARGS];
    int num_args;
    while (1)
    {
        // Receive message from client
        int read_size = recv(client_socket, client_message, 2000, 0);
        if (read_size == 0)
        {
            printf("Client disconnected\n");
            break;
        }
        // Add null terminator to message
        client_message[read_size] = '\0';
        printf("Client message: %s", client_message);
        char temp[1000];
        sprintf(temp, "%s", client_message);
        // call commands
        // break raw command
        num_args = 0;
        args[num_args] = strtok(temp, " \n");
        int file_transfer = 0;
        while (args[num_args] != NULL && num_args < MAX_ARGS - 1)
        {
            num_args++;
            args[num_args] = strtok(NULL, " \n");
        }
        // print arguments
        // for (int i = 0; i < num_args; i++)
        // {
        //     printf("args[%d]: %s\n", i, args[i]);
        // }

        // commands
        int command_success_flag = 0;
        char message[1000];
        if (strcmp(args[0], "findfile") == 0)
        {
            printf("findfiles invoked\n");
            file_transfer = 0;
            char *filename = args[1];
            char *path = getenv("HOME");
            int found = 0;
            findfile(client_socket, filename, path, &found);
            if (found == 0)
            {
                printf("file not found\n");
                memset(message, 0, sizeof(message));
                sprintf(message, "file not found\n");
                message[strlen(message)] = '\0';
                if (send(client_socket, message, strlen(message), 0) < 0)
                {
                    perror("send failed");
                    exit(EXIT_FAILURE);
                }
                memset(message, 0, sizeof(message));
            }
            // printf("message: %s", message);
        }

        else if (strcmp(args[0], "sgetfiles") == 0)
        {
            printf("sgetfiles invoked\n");
            file_transfer = 1;
            long size1 = atol(args[1]);
            long size2 = atol(args[2]);
            // if (size1 < 0 || size2 < 0 || size1 > size2)
            // {
            //     printf("Invalid size parameters\n");
            //     return 0;
            // }

            char home_dir[1024];
            snprintf(home_dir, 1024, "%s", getenv("HOME"));

            sgetfiles(home_dir, "temp.tar.gz", size1, size2);
            file_transfer = 1;
            // printf("out of function\n");
            // send_file(client_socket, "temp.tar.gz");
            // transfer file
            // Opening tar file
            // if ((file_fd = open("temp.tar.gz", O_RDONLY)) == -1)
            // {
            //     perror("open");
            //     exit(EXIT_FAILURE);
            // }

            // Reading and sending file content
            // ssize_t read_bytes;
            // while ((read_bytes = read(file_fd, buffer, BUFFER_SIZE)) > 0)
            // {
            //     if (send(client_socket, buffer, read_bytes, 0) == -1)
            //     {
            //         perror("send");
            //         exit(EXIT_FAILURE);
            //     }
            // }
            // close(file_fd);
            // int filefd, len;
            // fprintf(stderr, "Copying file to client %d\n", client_socket);
            // filefd = open("temp.tar.gz", O_RDONLY);
            // char buffer[BUFFER_SIZE];

            // while ((len = read(filefd, buffer, BUFFER_SIZE)) > 0)
            // {
            //     if (send(client_socket, buffer, len, 0) == -1)
            //     {
            //         perror("send");
            //         return -1;
            //     }
            // }
            // sleep(1);
            // fprintf(stderr, "File sent to client succesfully\n");
            // close(filefd);
            // close(socketFd);

            // char tar_filename[] = "temp.tar.gz";
            // sgetfiles(tar_filename, size1, size2);
        }

        else if (strcmp(args[0], "dgetfiles") == 0)
        {
            printf("dgetfiles invoked\n");
            struct tm date1_tm, date2_tm;
            memset(&date1_tm, 0, sizeof(date1_tm));
            memset(&date2_tm, 0, sizeof(date2_tm));
            if (strptime(args[1], "%Y-%m-%d", &date1_tm) == NULL || strptime(args[2], "%Y-%m-%d", &date2_tm) == NULL)
            {
                printf("Invalid date format: YYYY-MM-DD\n");
                // return 0;
            }

            time_t date1 = mktime(&date1_tm);
            time_t date2 = mktime(&date2_tm);
            // if (date1 == -1 || date2 == -1 || date1 > date2)
            // {
            //     printf("Invalid date parameters\n");
            //     return 0;
            // }

            char home_dir[1024];
            snprintf(home_dir, 1024, "%s", getenv("HOME"));

            dgetfiles(home_dir, "temp.tar.gz", date1, date2);
            file_transfer = 1;
            // transfer file
            // Opening tar file
            // char tar_filename[] = "temp.tar.gz";
            // dgetfiles(tar_filename, date1, date2);
        }

        else if (strcmp(args[0], "getfiles") == 0)
        {
            printf("getfiles invoked\n");
            char **files = malloc((num_args - 1) * sizeof(char *));
            for (int i = 1; i < num_args; i++)
            {
                files[i - 1] = args[i];
            }
            int num_files = num_args - 1;

            // if (num_args == 8 && strcmp(args[7], "-u") == 0)
            // {
            //     num_files--;
            // }
            if (strcmp(args[num_args - 1], "-u") == 0)
            {
                num_files--;
                // unzip_tar_file(tar_filename);
            }
            char tar_filename[] = "temp.tar.gz";
            int found = 0;
            getfiles(tar_filename, files, num_files, &found);
            file_transfer = 1;
            // if (found)
            // {
            //     file_transfer = 1;
            // }
            // else
            // {
            //     printf("No files found.\n");
            //     memset(message, 0, sizeof(message));
            //     sprintf(message, "No files found.\n");
            //     message[strlen(message)] = '\0';
            //     if (send(client_socket, message, strlen(message), 0) < 0)
            //     {
            //         perror("send failed");
            //         exit(EXIT_FAILURE);
            //     }
            //     memset(message, 0, sizeof(message));
            // }
        }

        else if (strcmp(args[0], "gettargz") == 0)
        {
            printf("gettarz invoked\n");
            char **extensions = malloc((num_args - 1) * sizeof(char *));
            for (int i = 1; i < num_args; i++)
            {
                extensions[i - 1] = args[i];
                // printf("extension %d: %s\n", i, args[i]);
            }
            int num_extensions = num_args - 1;

            if (num_args == 9 && strcmp(args[8], "-u") == 0)
            {
                num_extensions--;
            }

            char tar_filename[] = "temp.tar.gz";
            gettargz(tar_filename, extensions, num_extensions);
            file_transfer = 1;
            // // transfer file
            // // Opening tar file
            // if ((file_fd = open("temp.tar.gz", O_RDONLY)) == -1)
            // {
            //     perror("open");
            //     exit(EXIT_FAILURE);
            // }

            // // Reading and sending file content
            // ssize_t read_bytes;
            // while ((read_bytes = read(file_fd, buffer, BUFFER_SIZE)) > 0)
            // {
            //     if (send(client_socket, buffer, read_bytes, 0) == -1)
            //     {
            //         perror("send");
            //         exit(EXIT_FAILURE);
            //     }
            // }
            // close(file_fd);
        }

        else if (strcmp(args[0], "quit") == 0)
        {
            printf("Client requested to exit\n");
            break;
        }
        // Check for exit command
        // if (strcmp(client_message, "quit\n") == 0)
        // {
        //     printf("Client requested to exit\n");
        //     break;
        // }

        // Send message back to client
        // write(client_socket, "Server message: ", strlen("Server message: "));
        // write(client_socket, client_message, strlen(client_message));
        // write(client_socket, message, strlen(message));
        // send_file(client_socket, "temp.tar.gz");
        if (file_transfer)
        {
            printf("Transfering file....\n");
            send_file(client_socket, "temp.tar.gz");
        }
    }
    close(client_socket);
    // fclose(fp);
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

void handle_mirror(int client_socket)
{
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1)
    {
        // perror("Error: Failed to receive data from client");
        exit(1);
    }

    printf("Received data from client: %s\n", buffer);

    // Connect to mirror server
    int mirror_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mirror_socket == -1)
    {
        perror("Error: Failed to create mirror socket");
        exit(1);
    }

    struct sockaddr_in mirror_addr;
    mirror_addr.sin_family = AF_INET;
    mirror_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    mirror_addr.sin_port = htons(8081);

    if (connect(mirror_socket, (struct sockaddr *)&mirror_addr, sizeof(mirror_addr)) == -1)
    {
        perror("Error: Failed to connect to mirror server");
        exit(1);
    }

    // Send data to mirror server
    if (send(mirror_socket, buffer, bytes_received, 0) == -1)
    {
        perror("Error: Failed to send data to mirror server");
        exit(1);
    }

    // Receive data from mirror server
    bytes_received = recv(mirror_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1)
    {
        perror("Error: Failed to receive data from mirror server");
        exit(1);
    }

    printf("Received data from mirror server: %s\n", buffer);

    // Send data back to client
    if (send(client_socket, buffer, bytes_received, 0) == -1)
    {
        perror("Error: Failed to send data to client");
        exit(1);
    }

    // Close sockets
    close(mirror_socket);
    close(client_socket);
}

int main()
{

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    int clients_count = 0;
    int opt = 1;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error: Failed to create socket");
        exit(1);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Error: Failed to set 'setsockopt' for reusing address");
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);          // Port number for server

    // Bind the server address to the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error: Failed to bind socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Error: Failed to listen");
        exit(1);
    }

    printf("Server is listening for incoming connections...\n");

    while (1)
    {
        // Accept incoming connections
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            perror("Error: Failed to accept connection");
            exit(1);
        }

        printf("\nClient connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle the first 4 client connections in the server
        if (clients_count < 4 || (clients_count >= 8 && clients_count % 2 == 1))
        {
            // handle_client(client_socket);
            // clients_count++;
            pid_t pid = fork();
            if (pid == -1)
            {
                close(client_socket);
                perror("Error: Coult not fork to handle client");
                continue;
            }
            else if (pid == 0)
            {
                // Child process
                printf("Client %d handled by the servor\n", clients_count + 1);
                close(server_socket); // Close unused server socket
                processclient(client_socket);
                exit(EXIT_SUCCESS);
            }
            else
            {
                clients_count++; // Increment count of clients.
                printf("No. of Clients handled: %d\n", clients_count);
                // Parent process
                close(client_socket); // Close unused client socket
                printf("server closed client connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
        }

        // Handle the next 4 client connections in the mirror
        // else if (clients_count >= 4 && clients_count < 8) {
        else
        {

            // clients_count++;
            pid_t pid = fork();
            if (pid == -1)
            {
                close(client_socket);
                perror("Error: Coult not fork to handle client");
                continue;
            }
            else if (pid == 0)
            {
                // Child process
                printf("Client %d handled by the mirror\n", clients_count + 1);
                close(server_socket); // Close unused server socket
                handle_mirror(client_socket);
                exit(EXIT_SUCCESS);
            }
            else
            {
                clients_count++; // Increment count of clients.
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