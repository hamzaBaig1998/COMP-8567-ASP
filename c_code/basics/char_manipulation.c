#include <stdio.h>
#include <ctype.h>

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

    // ischar
    if (isalpha('a'))
    {
        printf("This is a character\n");
    }
    else
    {
        printf("This is not a character\n");
    }

    // isdigit
    if (isdigit('a'))
    {
        printf("This is a digit\n");
    }
    else
    {
        printf("This is not a digit\n");
    }
}