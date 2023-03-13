
#include <stdio.h>

int main(void){

int ar[4] = {5, 10, 15, 20};
int *ptr; int i;


printf("\n");

for (i=0; i<4; i++) 
{
printf("Using dereferencing %d\n", *(ar+i)); //Print the value of each array element using deferefencing
printf("Using array index %d\n", ar[i]);
}

printf("\n");

//Print the values of the array by assigning ptr=ar using dereferencing
ptr=ar;	/* Equivalent to ptr=&ar[0] */ 

for (i=0; i<4; i++)
{
printf("Using dereferencing %d\n", *(ptr+i));
printf("Using array index %d\n", ptr[i]);
}


}

