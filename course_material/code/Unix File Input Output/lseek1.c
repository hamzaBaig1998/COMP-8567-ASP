#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// lseek1.c with SEEK_SET

main()
{

    int fd3 = open("check.txt", O_RDWR);

    int long n = lseek(fd3, 20, SEEK_SET);

    printf("\nThe offset is %d\n", n);

    char buff1[15];
    for (int i = 0; i < 15; i++)
    {
        buff1[i] = 'A';
    }

    n = write(fd3, buff1, 15);

    printf("\nThe number of bytes written from the offset is %ld\n", n);

    close(fd3);
}
