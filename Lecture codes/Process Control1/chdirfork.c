#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int p,p1;
    p = fork();
    
    if(p==-1)
    {
        printf("There is an error while calling fork()");
    }
    if(p==0)
    {   
        printf("\nWe are in the child process\n");
       char s[100];
  
    // printing current working directory
    printf("The current working directory of the child process is %s\n", getcwd(s, 100));
  
    // using the command
    chdir("/home/pranga/Documents");
  
    // printing current working directory
    printf("The new working directory of the child process is %s\n", getcwd(s, 100));
  
        exit(0);
    }
    else
    {
      //int k=wait();
        
        printf("\nWe are in the parent process\n");
       
        
        exit(0);
    }
    return 0;
}
