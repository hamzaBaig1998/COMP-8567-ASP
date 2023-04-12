#include <stdlib.h>
#include <stdio.h> 

int main(void)
{

void *ptr; 
int a=100;
float b=35.5;

ptr=&a; 

//printf("The value of a is %d\n",*ptr);

int *p1;
p1=ptr;
printf("The value of a is %d\n",*p1);

ptr=&b;

float *p2;
p2=ptr;
printf("The value of a is %f\n",*p2);


}
