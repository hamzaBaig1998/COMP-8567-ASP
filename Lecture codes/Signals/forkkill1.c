#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(void)
{ 

int pid1=fork();
int pid2=fork();
int pid3=fork();

if(pid1>0 ||pid2>0 ||pid3>0) 
{
int i=0;
for(;;)
{
i++;
sleep(1);
printf("\n The parent id is %d\n",getpid()); 
if(i==5)
{
printf("\n Killing all the child process\n");
kill(0,SIGALRM);
sleep(2);
}
} 
printf("\n Killing myself-THe parent process\n");
kill(getpid(),SIGALRM);
} 
else 
{ 

for(;;)
{
sleep(1);
printf("\n The childid id is %d\n",getpid()); 
} 

} 

}

