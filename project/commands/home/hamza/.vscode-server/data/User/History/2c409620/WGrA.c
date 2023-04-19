#include <stdio.h>

void main()
{
    int c;

    printf("Enter a letter: ");
    c = getchar();
    printf("You entered:");
    putchar(c);
    putchar('\n');
}