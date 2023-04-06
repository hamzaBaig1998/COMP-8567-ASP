#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int p;
    int main_id=getpid();
    p = fork();
    p = fork(); 

    if(p==0){   
        if(getppid()!=main_id){
        printf("\nWelcome to COMP 8567\n"); 
        printf("The process ID is %d\n",getpid());
        }
       	}
    
    if(p==-1){
        printf("There is an error while calling fork()");
       }
    else
    {
    sleep(1);
    } 
    
   } 
