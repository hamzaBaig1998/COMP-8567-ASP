#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


   int main(void)
	{
	 int i=fork();

		
	if(i==0)
	{
	
	
	printf("\n\nCHILD PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	exit(EXIT_SUCCESS); //
	}
	else if(i<0)
	{
	printf("\n\nERROR\n");

	}
	else
	{
	int status;
	int cid=wait(&status);
	
	printf("\n\nPARENT PROCESS\n");
	printf("\n The status is %d\n",status);
	
	printf("\nThe child process %d has successfully completed execution and parent is resuming execution\n",cid);
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	exit(0);
	}
    }



