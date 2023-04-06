#include <stdio.h>
#include <stdlib.h>



int c;

int * divide(int a, int b) 
{
if (b==0)
return NULL;
else
{
c=a/b;
return &c;
}
} 


int main(int argc,char *argv[])
{
printf("\n Arguement one is %d \n", atoi(argv[1]));
printf("\n Arguement one is %d \n", atoi(argv[2]));
/*
int *ptr1=divide(int(argv[1]),int(argv[2]));
if(ptr1==NULL)
printf("THe division is not possible");
else
printf("a/b =%d",*ptr1);*/

return 0;
}
