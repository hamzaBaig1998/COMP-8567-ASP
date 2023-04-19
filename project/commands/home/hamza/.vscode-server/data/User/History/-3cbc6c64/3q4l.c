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

    // switch
    char option;
    printf("Q1. Who is the founder of Pakistan?\na. Allama Iqbal\nb. Quaid-e-Azam\nc. Abdul Qader Khan\nd. Sir Sayyed Ahmed Khan");
    scanf("%c", &option);
    switch (option)
    {
    case 'a':
        printf("Wrong Answer\n");
        break;
    case 'b':
        printf("You guessed it right!!!\n");
        break;
    case 'c':
        printf("Wrong Answer\n");
        break;
    case 'd':
        printf("Wrong Answer\n");
        break;
    default:
        printf("Please don't try to be oversmart and choose from the options given next time.");
        break;
    }
}