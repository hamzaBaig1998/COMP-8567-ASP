#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// calculates the min
int min(int *n1, int *n2, int *n3)
{
    int min = *n1;
    if (*n2 < min)
        min = *n2;
    if (*n3 < min)
        min = *n3;
    return min;
}

// calculates the max
int max(int *n1, int *n2, int *n3)
{
    int max = *n1;
    if (*n2 > max)
        max = *n2;
    if (*n3 > max)
        max = *n3;
    return max;
}

// product of the results of the two methods
int prod(int (*min)(int *, int *, int *), int (*max)(int *, int *, int *), int *n1, int *n2, int *n3)
{
    return (*min)(n1, n2, n3) * (*max)(n1, n2, n3);
}

int main()
{
    srand(time(0));
    // random values assigned
    int n1 = rand() % 100;
    int n2 = rand() % 100;
    int n3 = rand() % 100;

    printf("n1: %d, n2: %d, n3: %d\n", n1, n2, n3);
    // print the result
    int result = prod(&min, &max, &n1, &n2, &n3);
    printf("The product of min and max is: %d\n", result);
    return 0;
}