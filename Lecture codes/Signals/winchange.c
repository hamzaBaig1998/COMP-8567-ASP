#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

void AlarmHandler(){ 

printf("\nWindow is being resized\n"); 

}

int main(int argc, char *argv[]){
signal(SIGWINCH, AlarmHandler); //install a handler 
while(1) 
pause();
}

