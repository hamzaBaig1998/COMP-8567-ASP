#include <stdio.h>

void greet()
{
    printf("Hello World\n");
}

// pass by value
int sum(int a, int b)
{
    return a + b;
}

// pass by reference
int sum_by_ref(int *a, int *b)
{
    return *a + *b;
}

void greet_by_ref(char *name)
{
    printf("Hello %s\n", *name);
}

void main()
{
    int a = 5;
    int b = 10;

    greet();
    printf("Enter your name:");

    char name[64];
    fgets(name, 64, stdin);
    greet_by_ref(&name);
    int res = sum(12, 1);
    printf("Sum =%d\n", res);
    res = sum_by_ref(&a, &b);
    printf("Sum =%d\n", res);
}