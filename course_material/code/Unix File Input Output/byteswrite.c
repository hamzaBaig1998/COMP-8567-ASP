#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
// byteswrite.c

main()
{

    // Writes into check.txt from an array of characters

    int fd3 = open("check.txt", O_RDWR);
    char buff1[10] = "AAAAAAAAAA";

    long int n;

    n = write(fd3, buff1, 10);

    printf("\n\nThe number of bytes written into the file were %ld\n", n);

    char buff2[15] = "BBBBBBBBBB";

    n = write(fd3, buff2, 10);

    printf("\n\nThe number of bytes written into the file were %ld\n", n);

    close(fd3);
}
