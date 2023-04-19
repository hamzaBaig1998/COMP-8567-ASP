#include <stdio.h>

void greet()
{
    printf("Hello World");
}

// pass by value
int sum(int a, int b)
{
    return a + b;
}

// pass by reference
int sum(int *a, int *b)
{
    return *a + *b;
}

void main()
{
}