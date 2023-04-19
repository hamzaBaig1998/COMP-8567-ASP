#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGHT 10000

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
                // printf("searching file::::%s", files[i]);
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

void create_tar_file(char *tar_filename, char **files, int num_files)
{
    char complete_files_path[MAX_COMMAND_LENGHT] = ""; // Initialize to empty string
    char command[MAX_COMMAND_LENGHT];
    char file_path[MAX_PATH_LENGTH];

    search_directory(getenv("HOME"), files, num_files, complete_files_path);

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
}

void unzip_tar_file(char *tar_filename)
{
    char command[MAX_PATH_LENGTH + 10];
    sprintf(command, "tar -xzf %s -C %s", tar_filename, getenv("PWD"));
    int status = system(command);
    if (status == -1)
    {
        perror("Error executing tar command");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 8 || (argc == 2 && strcmp(argv[1], "-u") == 0))
    {
        printf("Usage: %s file1 [file2 ... file6] [-u]\n", argv[0]);
        return 0;
    }

    char **files = malloc((argc - 1) * sizeof(char *));
    for (int i = 1; i < argc; i++)
    {
        files[i - 1] = argv[i];
    }
    int num_files = argc - 1;

    // if (argc == 8 && strcmp(argv[7], "-u") == 0)
    // {
    //     num_files--;
    // }
    char tar_filename[] = "temp.tar.gz";
    if (strcmp(argv[argc - 1], "-u") == 0)
    {
        num_files--;
        unzip_tar_file(tar_filename);
    }
    for (int i = 0; i < num_files; i++)
    {
        printf("file:[%d]: %s\n", i, files[i]);
    }

    create_tar_file(tar_filename, files, num_files);

    free(files);
    return 0;
}