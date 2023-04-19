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

void main()
{
    int a = 5;
    int b = 10;

    greet();

    int res = sum(12, 1);
    printf("Sum =%d\n", res);
    res = sum_by_ref(&a, &b);
    printf("Sum =%d\n", res);
}