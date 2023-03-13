#include <stdio.h>
int print1()  
{
    printf("\nHello World from print1!\n");
    return 0;
}


int print2()  
{
    printf("\nHello World from print2!\n");
    return 0;
}


void helloworld(int (*fn)(),int (*fn1)())  
{

  printf("\n invoking print1 and print2 from within helloworld\n");
  
 
  fn();
  fn1();
   
}


int main(void)  
{
    helloworld(&print1,&print2);
    return (0);
}
