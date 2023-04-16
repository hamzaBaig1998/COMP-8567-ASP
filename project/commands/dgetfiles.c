/*

This implementation uses the opendir, readdir, and stat functions to traverse
the directory tree rooted at the user's home directory and filter files based
on their creation date. It then creates a tar file containing the matching files,
and optionally unzips it in the current working directory if the -u flag is present.
The program takes two required arguments, date1 and date2, in the format YYYY-MM-DD,
and an optional third argument -u.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGHT 10000

void create_tar_file(char *tar_filename, time_t date1, time_t date2)
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
        printf("Usage: %s date1 date2 [-u]\n", argv[0]);
        return 0;
    }

    struct tm date1_tm, date2_tm;
    memset(&date1_tm, 0, sizeof(date1_tm));
    memset(&date2_tm, 0, sizeof(date2_tm));
    if (strptime(argv[1], "%Y-%m-%d", &date1_tm) == NULL || strptime(argv[2], "%Y-%m-%d", &date2_tm) == NULL)
    {
        printf("Invalid date format: YYYY-MM-DD\n");
        return 0;
    }

    time_t date1 = mktime(&date1_tm);
    time_t date2 = mktime(&date2_tm);
    if (date1 == -1 || date2 == -1 || date1 > date2)
    {
        printf("Invalid date parameters\n");
        return 0;
    }

    char tar_filename[] = "temp.tar.gz";
    create_tar_file(tar_filename, date1, date2);

    if (argc == 4 && strcmp(argv[3], "-u") == 0)
    {
        unzip_tar_file(tar_filename);
    }

    return 0;
}
