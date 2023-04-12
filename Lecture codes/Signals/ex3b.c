#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void myAlarmHandler(int signum){
printf("I got a signal %d, I took care of it\n",signum); 
alarm(3);
}

int main(int argc, char *argv[]){

signal(SIGALRM, myAlarmHandler);//install handler 

alarm(3);	// for first time

while(1){
printf("I am working\n"); 
sleep(1);
}
}

