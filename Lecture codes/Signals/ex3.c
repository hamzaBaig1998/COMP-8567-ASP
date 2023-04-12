#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

//Replaces the default handler with a user-defined handler 

void myAlarmHandler(int signo){
//printf("Received the alarm signal %d, no action taken,returning to main\n",signo); 

//Additional action can be performed if required by the programmer
}

int main(int argc, char *argv[]){

//install handler 
signal(SIGALRM, myAlarmHandler);

//System alarm goes off 5 seconds from this point
alarm(5); 

while(1){
printf("I am working\n"); 
sleep(1);
}
}

