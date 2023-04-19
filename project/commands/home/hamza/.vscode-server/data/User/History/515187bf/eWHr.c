#include <stdio.h>

void main()
{
    char animals[4][5] = {
        "Ant",
        "Cat",
        "Dog",
    };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%c", animals[i][j]);
        }
        printf("\n");
    }
}