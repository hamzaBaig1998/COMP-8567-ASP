#include <stdio.h>

void main()
{

    //  if condition
    int x;
    int num = 25;

    printf("Hot or Cold\nEnter a number:");
    scanf("%d", &x);

    if (x == num)
    {
        printf("You guessed it right!!!\n");
    }
    else if (x <= num)
    {
        printf("Cold - You should aim higher\n");
    }
    else if (x >= num)
    {
        printf("Hot - You might wanna try lowering your number\n");
    }
}