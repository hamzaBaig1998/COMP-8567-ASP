#include<stdio.h>
#include<stdlib.h>
//Funtions as paramters

int min(int *n1,int *n2, int *n3){

	if(*n1<=*n2 && *n1<=*n3){
		return *n1;
	}
	else if(*n2<=*n1 && *n2<=*n3){
		return *n2;
	}
	else{
		return *n3;
	}
}

int max(int *n1,int *n2, int *n3){
	if(*n1>=*n2 && *n1>=*n3){
		return *n1;
	}
	else if(*n2>=*n1 && *n2>=*n3){
		return *n2;
	}
	else{
		return *n3;
	}
}

int prod (int (*min)(), int (*max)(), int *n1, int *n2, int *n3){

       return min(n1,n2,n3)*max(n1,n2,n3);
       
}

int main(){

        int num1,num2,num3;
        printf("\nEnter the values of num1,num2 and num3\n");
        scanf("%d %d %d",&num1,&num2,&num3); 

	int mmprod= prod (min, max, &num1, &num2, &num3);
	printf("Product of min and max of the numbers is: %d\n", mmprod);
	return 0;
}
