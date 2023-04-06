
#include <stdio.h>


int main(void) {

int num1=10, num2;   // 2 variables of type integer 

int *ptr;        //a pointer to integer

ptr = &num1;       // ptr = address of num1 

printf("\nThe address of num1 is %d\n", &num1);

num2 = *ptr;	//num2 = value stored at the address pointed by ptr, i.e value of num1 

printf("\n num1:%d  num2:%d  *ptr:%d \n\n", num1, num2, *ptr);      

printf("\nThe address currently stored in ptr is %d\n",ptr); 

printf("\nThe value of integer stored at the address pointed by ptr is %d\n",*ptr);

return(0);
}

