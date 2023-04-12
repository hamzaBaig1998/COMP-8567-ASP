#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void myAlarmHandler(int signo){}; // to avoid default action for SIGALRM (which is quitting) 

int main(int argc, char *argv[]){
pid_t pid;

if((pid=fork()) > 0){ //Parent Process 
printf("Parent Process: The child process is on pause and must wait for 10 seconds\n"); 
sleep(10);
printf("Parent Process:'Child process can now resume'\n"); 
kill(pid, SIGALRM); // wakes up the child process from the pause() state
//pid contains the proces id of the child process 
printf("Parent Exiting\n");
}
else{ //Child Process
printf("Child Proces: 'I have to wait for my parent process'\n"); 
signal(SIGALRM,	myAlarmHandler);
pause(); //child is paused and is waiting for a signal (any signal) 
printf("Child process is not resuming\n"); 
sleep(4);
printf("Child Exiting\n");
exit(0);
}

}

