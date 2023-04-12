#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void AlarmHandler(){ 
printf("\nIn the alarm handler\n");
}

int main(int argc, char *argv[])
{

signal(SIGALRM, AlarmHandler); //install the handler 

alarm(5);

printf("\nThe system is pausing\n");
int i=pause();
printf("\nThe process has resumed after pause\n");
printf("\nThe return value of pause() is %d\n",i);


}

