#include <stdio.h>
#include <unistd.h>


   int main(void)
	{
	 int i=fork();

		
	if(i==0)
	{
	for(;;)
	{
	printf("\n\nCHILD PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	}
	}
	else if(i<0)
	{
	printf("\n\nERROR\n");

	}
	else
	{
	wait(NULL);
	
	printf("\n\nPARENT PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	
	}
    }



