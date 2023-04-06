#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void AlarmHandler(){ 
static int n = 0; 
if(n++ < 6 ){
printf("Beeping  %c  %c  %c\n", 7, 7, 7); 
alarm(3);
}
else
exit(0);
}

int main(int argc, char *argv[]){
signal(SIGALRM, AlarmHandler); //install a handler 
AlarmHandler();
while(1) 
pause();
}

