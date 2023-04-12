#include <stdio.h>

//int (*m1)(int,int);
//int (*m2)(int);


int maximum(int a,int b)
{

if(a>b)
return a;
else
return b;
}


int fact(int c) 
{
int ret=1;
while(c>=1)
{
ret=ret*(c--);
}
return ret; 
}


int compute(int (*m1)(int,int),int (*m2)(int),int x1,int x2)
{

int ret1, ret2;

ret1=m1(x1,x2);
ret2=m2(ret1);

return ret2;
}



int main(void)
{

int ret3=compute(&maximum,&fact,5,3);

printf("\nThe factorial of max(x1,x2) is %d\n",ret3);

}








