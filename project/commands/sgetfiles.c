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
#include <sys/wait.h>

#define MAX_PATH_LEN 1024
#define MAX_CMD_LEN 2048

void create_tar_file(const char *dir_path, const char *tar_path, const char *size1, const char *size2)
{
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, MAX_CMD_LEN, "find %s -type f -size +%s -size -%s -print0 | xargs -0 tar -czf %s", dir_path, size1, size2, tar_path);
    system(cmd);
}

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4)
    {
        printf("Usage: %s size1 size2 [-u]\n", argv[0]);
        return 1;
    }

    char *size1 = argv[1];
    char *size2 = argv[2];
    int unzip = 0;

    if (argc == 4 && strcmp(argv[3], "-u") == 0)
    {
        unzip = 1;
    }

    char home_dir[MAX_PATH_LEN];
    snprintf(home_dir, MAX_PATH_LEN, "%s", getenv("HOME"));

    create_tar_file(home_dir, "temp.tar.gz", size1, size2);

    if (unzip)
    {
        char cmd[MAX_CMD_LEN];
        snprintf(cmd, MAX_CMD_LEN, "tar -xzf temp.tar.gz");
        system(cmd);
    }

    return 0;
}