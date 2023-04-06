#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void alarmhandler()
{
printf("\n Child not sensitive to CTRL c\n");
}

int main(void)
{ 

int pid1=fork();


if(pid1>0) 
{
int i=0;
for(;;)
{
sleep(1);
printf("\n The parent id is %d\n",getpid()); 

} 

} 
else 
{ 

for(;;)
{
signal(SIGINT,alarmhandler);
//pause();
//printf("\nChild Resuming\n");
sleep(1);
printf("\n The childid id is %d\n",getpid()); 
}

} 

}

