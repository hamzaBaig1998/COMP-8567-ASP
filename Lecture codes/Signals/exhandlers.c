#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

//Replaces the default handler with a user-defined handler 

void myhandler1(int signo){

printf("Received the signal %d at myhandler1,returning to main\n",signo); 
exit(0);
//Additional action can be performed if required by the programmer
}


void myhandler2(int signo){
printf("Received the alarm signal %d at myhandler2,returning to main\n",signo); 

//Additional action can be performed if required by the programmer
}

int main(int argc, char *argv[]){
void (*prevhandler)(int); 

//install handler 
signal(SIGINT, myhandler1);

for(int i=0;i<15;i++)
{
printf("\n Press CTR+C to check the response of the handler\n");
sleep(1);
}



}//End 

