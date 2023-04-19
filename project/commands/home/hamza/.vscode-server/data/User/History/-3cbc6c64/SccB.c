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
        printf("You guessed it right!!!");
    }
    else if (x <= num)
    {
        printf("Cold - You should aim higher");
    }
    else if (x >= num)
    {
        print("Hot - You might wanna try lowering your number");
    }
}