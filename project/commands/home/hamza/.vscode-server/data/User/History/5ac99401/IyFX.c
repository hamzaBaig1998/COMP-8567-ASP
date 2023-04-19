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
int sum_by_ref(int *a, int *b)
{
    return *a + *b;
}

void main()
{
    int a = 5;
    int b = 10;

    greet();

    sum(12, 1);
    sum_by_ref(&a, &b);
}