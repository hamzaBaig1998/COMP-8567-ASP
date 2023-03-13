
// C program to demonstrate waitpid()
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
 
void waitexample()
{
    int i, stat;
    
    int pid=fork();
    
    if(pid==0)
    {
    printf("\nThe pid of the child process is %d\n",getpid());
    int a=a/0;
    exit(EXIT_SUCCESS);
    }
    else if(pid<0)
    {
    //error
    }
    else
    {
    //Parent Process 
    int k=wait(&stat); 
    
    printf("\nThe pid of the parent process is %d\n",getpid());
    
    if (WIFEXITED(stat))
            printf("Child process %d normally terminated with status: %d\n",
                   pid, WEXITSTATUS(stat)); //Termination code of the child process that terminated normally
                   
    if (WIFSIGNALED(stat))
            printf("Child process %d abnormally terminated with status: %d\n",
                   pid, WTERMSIG(stat)); //Signal that casused the termination 
                   
    
    } 
    
       
}
 

int main()
{
    waitexample();
    return 0;
}
