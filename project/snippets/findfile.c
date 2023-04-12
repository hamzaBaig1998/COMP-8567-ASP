#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

void searchFile(char *fileName, char *path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char filePath[1024];
    int found = 0;

    if ((dir = opendir(path)) != NULL && found != 1)
    {
        while ((entry = readdir(dir)) != NULL && found != 1)
        {
            printf("Searching in folder: %s\n", entry->d_name);
            if (strcmp(entry->d_name, fileName) == 0)
            {
                strcpy(filePath, path);
                strcat(filePath, "/");
                strcat(filePath, fileName);
                stat(filePath, &fileStat);
                printf("File found: %s\n", filePath);
                printf("Size: %ld bytes\n", fileStat.st_size);
                printf("Created on: %s \n", ctime(&fileStat.st_mtime));
                found = 1;
                break;
            }
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
            {
                char subPath[1024];
                snprintf(subPath, sizeof(subPath), "%s/%s", path, entry->d_name);
                searchFile(fileName, subPath);
            }
        }
        closedir(dir);
    }
    if (found == 0)
    {
        printf("File not found\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    searchFile(argv[1], "/");
    return 0;
}
