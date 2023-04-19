#include <stdio.h>

void main()
{
    // multi-dimensional array of characters
    char animals[4][5] = {
        "Ant",
        "Cat",
        "Dog",
    };

    animals[1][2] = 'r';
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%c", animals[i][j]);
        }
        printf("\n");
    }

    // multi-dimensional array of digits
}