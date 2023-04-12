#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int n=0;

void AlarmHandler()
{ 
printf("In the alarm handler\n"); 
if(n++<3)
alarm(10);
else
return;

}


int main(int argc, char *argv[]){
int a;
signal(SIGALRM, AlarmHandler); //install a handler 
if(n<3)
AlarmHandler();
while(1) 
{
a=pause();
}

printf("\n The value of A is %d\n", a);

}

