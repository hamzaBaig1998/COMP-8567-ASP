#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 


void myAlarmHandler(){
printf("I received an alarm, I took care of it\n");
alarm(3);
}


int main(int argc, char *argv[]){
signal(SIGALRM, SIG_DFL);//install handler
alarm(3); // for first time
while(1){
printf("I am continuing working\n");
sleep(1);
}
}
