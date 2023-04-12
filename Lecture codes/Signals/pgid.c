#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Prints the group id of the child and the parent process

   int main(void)
	{
	 int i=fork();

		
	if(i==0)
	{
	
	
	printf("\n\nCHILD PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	printf("\n The group id of the current process is %d \n",getpgid(getpid()));
	exit(0);
	}
	else if(i<0)
	{
	printf("\n\nERROR\n");

	}
	else
	{

	printf("\n\nPARENT PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	printf("\n The group id of the current process is %d \n",getpgid(getpid()));
	exit(0);
	}
    }



