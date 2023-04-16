#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define BUFFER_SIZE 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

void findfile(char *str, int *found)
{
    char path[MAX_WORD_LEN];
    char filename[MAX_WORD_LEN];
    char *words[MAX_WORDS];
    int word_count = 0;

    // Split the string into words using space as a delimiter
    char *token = strtok(str, " ");
    while (token != NULL && word_count < MAX_WORDS)
    {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    if (word_count != 2)
    {
        printf("Error: expected two arguments (filename and path)\n");
        return;
    }

    // Copy the filename and path from the array of words
    strcpy(filename, words[0]);
    strcpy(path, words[1]);

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
                printf("File Path:%s\nFilename:%s\nFile Size:%ld\nCreate At:%s\n", path, filename, st.st_size, ctime(&st.st_mtime));
                // Set the found flag
                *found = 1;
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
            findfile(str, found);
            // If the file is found, break out of the loop
            if (*found)
            {
                break;
            }
        }
    }

    // Close the directory
    closedir(dir);
}

int main()
{
    char str[MAX_WORDS * MAX_WORD_LEN];
    printf("Enter a filename and path separated by a space: ");
    fgets(str, sizeof(str), stdin);
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0'; // remove the newline character
    }

    int found = 0;
    findfile(str, &found);

    if (!found)
    {
        printf("File not found\n");
    }

    return 0;
}