/*
This implementation uses the opendir, readdir, and stat functions to traverse
the directory tree rooted at the user's home directory and filter files based
on their size. It then creates a tar file containing the matching files, and
optionally unzips it in the current working directory if the -u flag is present.
The program takes two required arguments, size1 and size2, and an optional third argument -u.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 4096

void create_tar_file(char *tar_filename, long size1, long size2)
{
    FILE *tar_file = fopen(tar_filename, "wb");
    if (!tar_file)
    {
        perror("Error creating tar file");
        exit(1);
    }

    DIR *root_dir = opendir(getenv("HOME"));
    if (!root_dir)
    {
        perror("Error opening root directory");
        exit(1);
    }

    struct dirent *dir_entry;
    char file_path[MAX_PATH_LENGTH];
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
            FILE *file = fopen(file_path, "rb");
            if (!file)
            {
                perror("Error opening file");
                exit(1);
            }

            char header[100];
            sprintf(header, "%s\n", dir_entry->d_name);
            fwrite(header, sizeof(char), strlen(header), tar_file);

            char size_str[20];
            sprintf(size_str, "%ld\n", file_stats.st_size);
            fwrite(size_str, sizeof(char), strlen(size_str), tar_file);

            char buffer[4096];
            size_t read_bytes;
            while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
            {
                fwrite(buffer, 1, read_bytes, tar_file);
            }

            fclose(file);
        }
    }

    closedir(root_dir);
    fclose(tar_file);
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
    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s size1 size2 [-u]\n", argv[0]);
        return 0;
    }

    long size1 = atol(argv[1]);
    long size2 = atol(argv[2]);
    if (size1 < 0 || size2 < 0 || size1 > size2)
    {
        printf("Invalid size parameters\n");
        return 0;
    }

    char tar_filename[] = "temp.tar.gz";
    create_tar_file(tar_filename, size1, size2);

    if (argc == 4 && strcmp(argv[3], "-u") == 0)
    {
        unzip_tar_file(tar_filename);
    }

    return 0;
}