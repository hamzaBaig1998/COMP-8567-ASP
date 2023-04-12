#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void TTIN_handler(){
printf("Attempted to read from keyboard\n");
exit(24);
}


int main(int argc, char *argv[])
{ 
int i, status, pid;
if((pid=fork())==0)//child
{ 
//signal(SIGTTIN, TTIN_handler); 
setpgid(0, getpid()); 
printf("Enter a value> \n"); 
scanf("%d", &i);
exit(0);
}
else  //parent
{
wait(&status); 
if(WIFEXITED(status))
printf("Exit status= %d\n",WEXITSTATUS(status));
else
printf("signaled by =  %d\n",WTERMSIG(status));
}
}

