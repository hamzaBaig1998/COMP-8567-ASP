#include <stdio.h>

void main()
{
    // for loop
    printf("For Loop\n");
    for (int i = 0; i < 10; i++)
    {
        printf("Item: %d\n", i);
    }

    // while
    printf("While loop");
    int num = -10;
    while (num <= 20)
    {
        if (num % 2 == 0)
        {
            printf("%d is even\n", num);
        }
        num++;
    }
}