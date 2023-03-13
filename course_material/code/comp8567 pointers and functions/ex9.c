#include <stdio.h>


	main(){
	int ar[4] = {5, 10, 15, 20};
	int *ptr;
	int i;
		for (i=0; i<4; i++)
		printf("\n%d", ar[i]);
		
		ptr=ar;
		
		for (i=0; i<4; i++)
		printf("\n%d", ptr[i]);
		
		int *p1;
		for(i=0;i<10;i++)
		{
		*(p1+i)=i;
		} 
		
		for(i=0;i<10;i++)
		{
		printf("\n%d\n", *(p1+i));
		} 
		
		}
		
		
		
		


