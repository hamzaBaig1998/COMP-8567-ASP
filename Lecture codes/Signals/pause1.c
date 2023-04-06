#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int n=0;

void AlarmHandler()
{ 
printf("\nWe are now in the handler\n");
}


int main(int argc, char *argv[]){
int a;
signal(SIGALRM, AlarmHandler); //install a handler 

alarm(5); 

printf("\nThe program has been paused for 5 seconds\n");
a=pause();

printf("\nThe program has been resumed\n");

}

