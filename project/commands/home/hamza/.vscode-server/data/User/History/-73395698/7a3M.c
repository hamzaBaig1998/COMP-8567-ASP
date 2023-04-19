#include <stdio.h>

void main()
{
    char str[] = "programmers";

    printf("We are all great %s\n", str);

    char name[64];

    printf("Enter your name: ");
    fgets(name, 64, stdin);
    printf("Hello %s", name);
}