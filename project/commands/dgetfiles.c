#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_PATH_LEN 1024
#define MAX_CMD_LEN 2048

void create_tar_file(const char *dir_path, const char *tar_path, const char *date1, const char *date2)
{
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, MAX_CMD_LEN, "find %s -type f -newermt '%s' ! -newermt '%s' -print0 | xargs -0 tar -czf %s", dir_path, date1, date2, tar_path);
    system(cmd);
}

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5)
    {
        printf("Usage: %s date1 date2 [-u]\n", argv[0]);
        return 1;
    }

    char *date1 = argv[1];
    char *date2 = argv[2];
    int unzip = 0;

    if (argc == 5 && strcmp(argv[4], "-u") == 0)
    {
        unzip = 1;
    }

    char home_dir[MAX_PATH_LEN];
    snprintf(home_dir, MAX_PATH_LEN, "%s", getenv("HOME"));

    create_tar_file(home_dir, "temp.tar.gz", date1, date2);

    if (unzip)
    {
        char cmd[MAX_CMD_LEN];
        snprintf(cmd, MAX_CMD_LEN, "tar -xzf temp.tar.gz");
        system(cmd);
    }

    return 0;
}