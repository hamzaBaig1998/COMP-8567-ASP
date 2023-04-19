#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // umask before creating file
    umask(0);
    int file = open("sample.txt", O_CREAT | O_WRONLY, 0777);

    // If error occurs
    if (file == -1)
    {
        puts("There was an error creating the file");
    }
    else
    {
        // Write to file in Write Only mode
        puts("Sample.txt has been created successfully");
        char *txt = "Welcome to COMP 8567";
        write(file, txt, strlen(txt));

        printf("%s written to sample.txt (%ld bytes)\n", txt, strlen(txt));
        close(file);

        // Append to file in Write Only mode
        file = open("sample.txt", O_APPEND | O_WRONLY);
        txt = "Advanced Systems Programming";
        write(file, txt, strlen(txt));
        printf("%s written to file (%ld bytes)\n", txt, strlen(txt));
        close(file);

        // Opening file in Read Write mode
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
        char temp;
        lseek(file, 0, SEEK_SET);
        while (read(file, &temp, 1) > 0)
        {
            // if temp contains null character
            if (temp == '\0')
            {
                // bring cursor to previous character
                lseek(file, -1, SEEK_CUR);
                // write @ to replace the null character
                write(file, "@", 1);
            }
        }
        puts("Replaced All the null characters with @");
        close(file);

        file = open("sample.txt", O_RDONLY);

        // Reading file
        char buffer[11];
        int bytes;
        // counter for size
        int file_size = 0;

        // print the contents of the file 10 bytes at a time (line by line)
        while ((bytes = read(file, buffer, 10)) > 0)
        {
            // replace last element with null if last element is less then 10
            buffer[bytes] = '\0';
            printf("%s\n", buffer);
            file_size = file_size + bytes;
        }

        // file_size+1 for the null character appended at the end of file.
        printf("Size of file:%d bytes\n", file_size + 1);
    }

    close(file);
    return 0;
}