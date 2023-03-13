#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PID_LEN 16
int main()
{
    int a = fork();
    fork();
    fork();

    if (a == 0)
    {
        // printf("parent killed \n");
        // exit(1);
    }
    while (1)
    {
        sleep(1);
    }
}