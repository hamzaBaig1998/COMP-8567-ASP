#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{

    // Reads from check.txt into an array of characters

    int fd3 = open("check.txt", O_RDONLY);
    char buff1[15];
    long int n;

    n = read(fd3, buff1, 15);
    printf("\n\nThe number of bytes read were %ld\n", n);

    for (int i = 0; i < 15; i++)
    {
        printf("%c", buff1[i]);
    }

    n = read(fd3, buff1, 15);
    printf("\n\nThe number of bytes read were %ld\n", n);

    for (int i = 0; i < 15; i++)
    {
        printf("%c", buff1[i]);
    }

    close(fd3);
}
