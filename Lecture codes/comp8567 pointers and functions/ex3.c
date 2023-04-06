#include <stdio.h> 

int main(void)
{ 

	int a=100;
	
	int *p=&a;
	
	printf("\nThe address of p %d\n",p); 
	p=p+1;
	printf("\nThe address of p++ is %d\n",p);
	p=p-1;
	printf("\nThe address of p-- is %d\n",p);
	p=p+2;
	printf("\nThe address of p+2 is %d\n",p); 
	p=p-2;
	printf("\nThe address of p-2 is %d\n",p);
}
