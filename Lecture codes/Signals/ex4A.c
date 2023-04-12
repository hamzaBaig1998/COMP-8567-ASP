#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void handler1(int signo){ 
printf("\nIn the user-defined handler for CTR+C\n");
}

int main(int argc, char *argv[])
{

signal(SIGINT, handler1); //install the handler 


printf("\nThe system is pausing and is waiting for a CTR+C signal\n");
int i=pause();
printf("\nThe process has resumed after the pause\n");
printf("\nThe return value of pause() is %d\n",i);


}

