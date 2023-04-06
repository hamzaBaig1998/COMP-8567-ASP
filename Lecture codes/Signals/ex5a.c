#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 


int main(int argc, char *argv[]){
pid_t pid;

if((pid=fork()) > 0){ //Parent Process 
int c=0;
printf("\nThe child process will be killed in 10 seconds\n");
for(;;)
{
printf("\nParent Process with PID %d is currently runnning\n",getpid());
sleep(1);
printf("\n %d second/s",++c);
if(c==10)
{kill(pid, SIGKILL); //Kill child process
printf("\nChild Process successfully terminated\n"); 
}
}

}
else{ //Child Process
for(;;)
{
printf("Child Process with PID %d is currently runnning\n",getpid());
sleep(1);
} 

}

}//End main

