#include <stdio.h>
#include <string.h>

void main()
{
    char string[] = "I love programming";
    int len;

    len = strlen(string);
    printf("The lenth of string: '%s' is :%d\n", string, len);
}