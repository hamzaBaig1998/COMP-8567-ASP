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
    // print using nested loop
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%c", animals[i][j]);
        }
        printf("\n");
    }
    // print using single loop
    for (int i = 0; i < 4; i++)
    {
        printf("%s", animals[i]);
    }

    // multi-dimensional array of digits
    int board[3][3];

    // initialize array
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = 0;
        }
    }
    board[1][1] = 1;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
}