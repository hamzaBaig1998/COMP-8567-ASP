#include <stdio.h>
#include <string.h>

void main()
{
    char string[] = "I love programming";
    int len;

    len = strlen(string);
    printf("The lenth of string: '%s' is :%d\n", string, len);

    // concatinating two strings in c
    char string_one[] = "I have a new house";
    char string_two[] = ", in my imagination";
    char buffer[64];

    strcpy(buffer, string_one);
    strcat(buffer, string_two);
    puts(buffer);
}