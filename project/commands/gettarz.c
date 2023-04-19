/*

This implementation uses the opendir and readdir functions to search the
directory tree rooted at the user's home directory for files with the specified
extensions. It then creates a tar file containing any matching files, and optionally unzips
it in the current working directory if the -u flag is present. The program takes at least one
required argument, extension1, and up to six additional optional arguments extension2 through
extension6, followed by an optional -u flag. If none of the specified extensions match any
files, the program prints "No file found" and exits.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGHT 10000

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

void create_tar_file(char *tar_filename, char **extensions, int num_extensions)
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
    char tar_filename[] = "temp.tar.gz";
    if (argc < 2 || argc > 9 || (argc == 2 && strcmp(argv[1], "-u") == 0))
    {
        printf("Usage: %s extension1 [extension2 ... extension6] [-u]\n", argv[0]);
        return 0;
    }

    char **extensions = malloc((argc - 1) * sizeof(char *));
    for (int i = 1; i < argc; i++)
    {
        extensions[i - 1] = argv[i];
        // printf("extension %d: %s\n", i, argv[i]);
    }
    int num_extensions = argc - 1;

    // if (argc == 9 && strcmp(argv[8], "-u") == 0)
    // {
    //     num_extensions--;
    // }
    if (strcmp(argv[argc - 1], "-u") == 0)
    {
        num_extensions--;
        unzip_tar_file(tar_filename);
    }

    create_tar_file(tar_filename, extensions, num_extensions);

    // if (argc == 9 && strcmp(argv[8], "-u") == 0)
    // {
    //     unzip_tar_file(tar_filename);
    // }

    free(extensions);
    return 0;
}
