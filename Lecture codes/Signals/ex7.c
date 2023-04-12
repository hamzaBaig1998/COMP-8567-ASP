#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void myAlarmHandler(int signo){}; // to avoid quiting 

int main(int argc, char *argv[]){
pid_t pid;

if((pid=fork()) > 0){ //Parent Process 
printf("Parent Process:Sleeps for a four seconds and then kills the child process\n"); 
sleep(4);
printf("\nNow killing the child process\n");
kill(pid, SIGALRM);
}
else{ //Child Process

signal(SIGALRM,SIG_DFL);
for(;;)
{
printf("Child Process is now Running\n"); 
}

}

}

