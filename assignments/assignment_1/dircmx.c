// Code by Hamza Baig (110089314)

// Source => Below Line: https://stackoverflow.com/questions/31787633/how-to-specify-nftw-flags
#define _XOPEN_SOURCE 500

// Including all header files required for this assignment
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

// Max 6 extensions allowed
#define MAX_EXT 6
#define MAX_PATH 256

char src[MAX_PATH];
char dst[MAX_PATH];
char ext[MAX_EXT][5];
// size of extension array
int num_ext = 0;
// flag for move or copy
int move = 0;
// flag for first folder
int init = 0;

// This function will be used to create the destination folder if it does not exist.
int create_folder_if_not_existing(const char *path)
{
    struct stat st;
    if (!(stat(path, &st) == 0 && S_ISDIR(st.st_mode)))
    {
        // mkdir system call for making directory with read write permissions
        int status = mkdir(path, 0775);
        if (status == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

// This function will check if a file has the desired extension
int check_extension(const char *path)
{
    int i;
    char *p = strrchr(path, '.');
    if (p == NULL)
    {
        return 0;
    }
    for (i = 0; i < num_ext; i++)
    {
        // printf("p: %s and extension at index: %s\n", p, ext[i]);
        if (strcmp(p + 1, ext[i]) == 0)
        {
            // printf("extension matched: %s\n", ext[i]);
            return 1;
        }
    }
    return 0;
}

int copy_func(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    char dst_path[MAX_PATH];
    int rv;

    // copy strings into dst_path
    // LOGIC: replacing the main src folder with the main dst folder in path, making it easy to copy or move files
    sprintf(dst_path, "%s%s", dst, path + strlen(src));

    if (typeflag == FTW_F)
    {
        // if file
        if (num_ext == 0)
        {
            // copy all files
            // printf("copy %s %s\n", path, dst_path);
            // creating a hard link, both file paths will refer to the same file on the file system
            rv = link(path, dst_path);
            if (rv == -1)
                perror("link");
        }
        else
        {
            // Copy files only if extension matches
            if (check_extension(path))
            {
                // printf("copy %s %s\n", path, dst_path);
                // creating a hard link, both file paths will refer to the same file on the file system
                rv = link(path, dst_path);
                if (rv == -1)
                    perror("link");
            }
        }
    }
    else if (typeflag == FTW_D)
    {
        // if directory
        // printf("Creating folder from (src): %s to destination (dst): %s \n", path, dst_path);
        // create folder here

        if (init == 0)
        {
            // skip main folder
            init++;
        }
        else
        {
            // Create the destination directory if it doesn't exist
            rv = mkdir(dst_path, 0775);
            if (rv == -1 && errno != EEXIST)
                perror("mkdir");
        }
    }
    return 0;
}

// Function to copy files and directories
int copy_directory(const char *src, const char *dst)
{
    return nftw(src, copy_func, 10, FTW_PHYS);
}

// Function to move files and directories
int move_directory(const char *src, const char *dst)
{
    int move_flag;

    // Copy the directory
    move_flag = copy_directory(src, dst);
    if (move_flag == -1)
        return move_flag;

    // Remove the source directory uses system's remove call
    move_flag = nftw(src, remove, 10, FTW_DEPTH | FTW_PHYS);
    if (move_flag == -1)
        perror("remove");

    return move_flag;
}

int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        // Atleast it should be ./dircmx destination_1 destination_2 option
        printf("Usage: %s [source_dir] [destination_dir] [-cp | -mv] [extension list]\n", argv[0]);
        return 1;
    }

    // Get source and destination directories
    strcpy(src, argv[1]);
    strcpy(dst, argv[2]);

    // Show error message if source file does not exist
    struct stat st;
    if (!(stat(src, &st) == 0 && S_ISDIR(st.st_mode)))
    {
        printf("Usage: %s [source_dir] [destination_dir] [-cp | -mv] [extension list]\nError: Source file should be exist!\n", argv[0]);
        return 1;
    }

    // Create destination folder if it does not exist
    create_folder_if_not_existing(dst);

    // Get the operation to perform: copy or move
    if (strcmp(argv[3], "-cp") == 0)
    {
        move = 0;
    }
    else if (strcmp(argv[3], "-mv") == 0)
    {
        move = 1;
    }
    else
    {
        // In case some other operation given (other than cp or mv)
        printf("Usage: %s [source_dir] [destination_dir] [-cp | -mv] [extension list]\nError: Invalid operation\n", argv[0]);
        return 1;
    }

    // Get the desired extensions
    if (argc > 4)
    {
        // only 6 extensions will be picked up
        for (int i = 4; i < argc && i - 4 < 6; i++)
        {
            strcpy(ext[i - 4], argv[i]);
            num_ext++;
        }
    }

    // Perform the copy or move operation
    if (move)
    {
        // Invoke the Move function
        return move_directory(src, dst);
    }
    else
    {
        // Invoke the copy function
        // puts("Copying files");
        return copy_directory(src, dst);
    }
}