#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    umask(0);
    int file = open("sample.txt", O_CREAT | O_WRONLY, 0777);

    // If error occurs
    if (file == -1)
    {
        puts("There was an error creating the file");
    }
    else
    {
        // Write to file
        puts("Sample.txt has been created successfully");
        char *txt = "Welcome to COMP 8567";
        write(file, txt, strlen(txt));
        printf("%s written to file (%d bytes)\n", txt, strlen(txt));
        close(file);

        // Append to file
        file = open("sample.txt", O_APPEND | O_WRONLY);
        txt = "Advanced Systems Programming";
        write(file, txt, strlen(txt));
        printf("%s written to file (%ld bytes)\n", txt, strlen(txt));
        close(file);

        file = open("sample.txt", O_RDWR);

        // 1. 5 bytes from the beginning of the file
        lseek(file, 5, SEEK_SET);
        write(file, "*****", 5);

        // 2. 4 bytes after the previous operation is performed
        lseek(file, 4, SEEK_CUR);
        write(file, "*****", 5);

        // 3. 10 bytes after the end of the file
        lseek(file, 0, SEEK_END);
        lseek(file, 10, SEEK_CUR);
        write(file, "*****", 5);

        // 4. replace all the NULL characters in the file with '@' character
        char c;
        lseek(file, 0, SEEK_SET);
        while (read(file, &c, 1) > 0)
        {
            if (c == '\0')
            {
                lseek(file, -1, SEEK_CUR);
                write(file, "@", 1);
            }
        }
        printf("Text written to file in O_RDWR mode\n");
        close(file);

        file = open("sample.txt", O_RDONLY);

        // Reading file
        char buffer[10];
        int byteSize;
        // counter for size
        int size = 0;

        // print the contents of the file 10 bytes at a time (line by line)
        while ((byteSize = read(file, buffer, 10)) > 0)
        {
            // buffer[byteSize] = '\0';
            printf("%s\n", buffer);
            size = size + byteSize;
        }

        printf("Size of file:%d bytes\n", size);
    }

    close(file);
    return 0;
}