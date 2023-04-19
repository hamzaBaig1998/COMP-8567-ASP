#include <stdio.h>

void main()
{
    // tolower as digit
    printf("this is a lower case example %d\n", tolower('A'));
    // tolower as character
    printf("this is a lower case example %c\n", tolower('A'));

    // toupper as digit
    printf("this is a upper case example %d\n", toupper('a'));
    // toupper as character
    printf("this is a upper case example %c\n", toupper('a'));
}