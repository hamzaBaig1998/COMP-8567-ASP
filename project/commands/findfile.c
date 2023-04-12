/*

The search_file function recursively searches for the specified filename in the directory tree rooted at path.
If the file is found, it prints the filename, size (in bytes), and date created to the console.
If the file is not found, it returns without printing anything.

In the main function, the program checks that the correct number of arguments have been supplied,
and then calls search_file with the specified filename and the user's home directory (getenv("HOME")).
If the file is not found, it prints "File not found" to the console.

To compile this program, you can use a command like:

gcc -o findfile findfile.c
Then you can run the program with a command like:

./findfile sample.txt


*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define BUFFER_SIZE 1024

void search_file(char *filename, char *path, int *found)
{
    DIR *dir;
    struct dirent *dp;
    struct stat st;
    char buf[BUFFER_SIZE];

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, filename) == 0)
        {
            sprintf(buf, "%s/%s", path, filename);
            if (stat(buf, &st) == 0)
            {
                printf("File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                *found = 1;
                break;
            }
        }
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            sprintf(buf, "%s/%s", path, dp->d_name);
            search_file(filename, buf, found);
            if (*found)
            {
                break;
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    char *path = getenv("HOME");

    int found = 0;
    search_file(filename, path, &found);

    if (!found)
    {
        printf("File not found\n");
    }

    return 0;
}