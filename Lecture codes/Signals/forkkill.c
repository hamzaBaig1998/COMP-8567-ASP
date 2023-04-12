#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(void)
{ 

int pid=fork();

if(pid>0) 
{
int i=0;
for(;;)
{
i++;
sleep(1);
printf("\n The parent id is %d\n",getpid()); 
if(i==5)
{
printf("\n Killing the child process\n");
kill(pid,SIGALRM);
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

