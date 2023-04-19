/*

The search_file function recursively searches for the specified filename in the directory tree rooted at path.
If the file is found, it prints the filename, size (in bytes), and date created to the console.
If the file is not found, it returns without printing anything.

In the main function, the program checks that the correct number of arguments have been supplied,
and then calls search_file with the specified filename and the user's home directory (getenv("HOME")).
If the file is not found, it prints "File not found" to the console.

In this version of the code, the search_file function now takes a pointer to an int variable found,
which is used to keep track of whether the file has been found or not. If the file is found,
search_file sets *found to 1 and breaks out of the loop. The main function now checks the value
of found after the search is complete, and prints "File not found" only if found is 0.

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
            search_file(filename, buf, found);
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

int main(int argc, char *argv[])
{
    // Check that the correct number of arguments have been supplied
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    // Get the target filename and the home directory
    char *filename = argv[1];
    char *path = getenv("HOME");

    // Search for the file
    int found = 0;
    search_file(filename, path, &found);

    // If the file was not found, print an error message
    if (!found)
    {
        printf("File not found\n");
    }

    return 0;
}