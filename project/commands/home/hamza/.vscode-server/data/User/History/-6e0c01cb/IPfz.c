#include <stdio.h>

void main()
{
    // for loop
    printf("For Loop:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("Item: %d\n", i);
    }

    // while
    printf("While loop:\n");
    int num = -10;
    while (num <= 20)
    {
        if (num % 2 == 0)
        {
            printf("%d is even number\n", num);
        }
        num++;
    }

    // Do while loop
    printf("Do while loop:\n");
    int flag = 1;
    int score = 10;
    do
    {
        printf("score: %d\n", score--);
    } while (flag == 1);
}