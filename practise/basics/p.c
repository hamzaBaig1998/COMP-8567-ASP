#include <stdio.h>
// ex1.c
int main(void)
{
    int n1 = 10, n2; // 2 variables of type integer
    float val = 4.3;
    int *ptr;  // a pointer to integer
    ptr = &n1; // ptr = address of num1
    n2 = *ptr; // n2 = value stored at the address pointed by ptr, i.e value of n1
    // ptr = &val; //This statement will give error due to type mismatch.
    // printf("\n :%d n2:%d *ptr:%d \n\n", n1, n2, *ptr);
    printf("\n n1:%d n2:%d *ptr:%d \n\n", n1, n2, *ptr);

    // Pointer arithmetics
    // p=p+2 means p = p + (2x4)
    int a = 100;
    int *p = &a;
    printf("\nThe address of p1 %d\n", p);
    p = p + 1;
    printf("\nThe address of p1+1 is %d\n", p);
    p = p - 1;
    printf("\nThe address of p1-1 is %d\n", p);
    p = p + 2;
    printf("\nThe address of p1+2 is %d\n", p);
    p = p - 2;
    printf("\nThe address of p1-2 is %d\n", p);

    return (0);
}