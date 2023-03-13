#include <stdio.h>
#include <unistd.h>


   int main(void)
	{
	int i=fork();	
	if(i==0)
	{
	printf("\n\nCHILD PROCESS\n");
	//char *s=get_current_dir_name();
	//printf("The current directory is %s\n",s);
	const char *s1="\home\pranga\Documents";
	chdir(&s1);
	printf("The new directory after chdir() is %s\n",get_current_dir_name());
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	}
	else if(i<0)
	{
	printf("\n\nERROR\n");

	}
	else
	{
	//wait(NULL);
	printf("\n\nPARENT PROCESS\n");
	printf("\n The id of the current process is %d \n",getpid());	
	printf("\n The parent id of the current process is %d \n",getppid());
	}
    }



