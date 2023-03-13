#include<stdio.h>
#include<stdlib.h>

int main(){

	int arr_size;
	printf("Enter the array size:\n");
	scanf("%d",&arr_size);
	int *ar;
	if((ar = malloc(sizeof(int)*arr_size))==NULL){
	printf("\nError\n");
	exit(1); 
	}
	
	for(int i=0; i<arr_size; i++){
	
		printf("Enter the %d element:\n",i);
		scanf("%d",&ar[i]);
	}
	
	
	printf("\nPrinting the array elements in the reverese order\n");
	
	for(int i=arr_size-1; i>=0; i--){
	
		printf("element %d is: %d\n",i,*(ar+i));
	}
	
	return 0;
    }
