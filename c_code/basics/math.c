#include <stdio.h>
#include <math.h>

int main()
{
    int x = 12;

    float s = sqrt(12);

    printf("The square root of %d is %f \n", x, s);

    float p;
    p = pow(2.0, 8.0);

    printf("2 powers 8 is %f\n", p);

    float a, b;

    printf("Enter value of a:");
    scanf("%f", &a);
    printf("Enter value of b:");
    scanf("%f", &b);

    printf("The addition of %f and %f is %f\n", a, b, a + b);
    printf("The subtraction of %f and %f is %f\n", a, b, a - b);
    printf("The multiplication of %f and %f is %f\n", a, b, a * b);
    printf("The division of %f and %f is %f\n", a, b, a / b);
    printf("%f power of %f is %f\n", a, b, pow(a, b));
    printf("The square root of %f is %f\n", a, sqrt(a));
    printf("The square root of %f is %f\n", b, sqrt(b));

    return (0);
}