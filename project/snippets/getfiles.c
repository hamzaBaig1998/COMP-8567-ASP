/*

This implementation uses the opendir, readdir, and stat functions to search the directory
tree rooted at the user's home directory for the specified files. It then creates a tar
file containing any matching files, and optionally unzips it in the current working directory
if the -u flag is present. The program takes at least one required argument, file1,
and up to six additional optional arguments file2 through file6, followed by an optional -u flag.
If none of the specified files are found, the program prints "No file found" and exits.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 4096

void create_tar_file(char *tar_filename, char **files, int num_files)
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

                struct stat file_stats;
                if (stat(file_path, &file_stats) == -1)
                {
                    perror("Error getting file stats");
                    exit(1);
                }

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
    }

    closedir(root_dir);
    fclose(tar_file);

    if (found_files == 0)
    {
        printf("No file found\n");
        exit(0);
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
    for (int i = 1; i < argc - 1; i++)
    {
        files[i - 1] = argv[i];
    }
    int num_files = argc - 2;

    if (argc == 8 && strcmp(argv[7], "-u") == 0)
    {
        num_files--;
    }

    char tar_filename[] = "temp.tar.gz";
    create_tar_file(tar_filename, files, num_files);

    if (argc == 8 && strcmp(argv[7], "-u") == 0)
    {
        unzip_tar_file(tar_filename);
    }

    free(files);
    return 0;
}
