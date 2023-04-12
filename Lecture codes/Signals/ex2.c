#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(int argc, char *argv[]){
void (*oldHandler1)(int); //to save default handlers 
void (*oldHandler2)(int); //for CTR-C and CTR-Z

oldHandler1=signal(SIGINT,SIG_IGN); //ignore CTR-C 

oldHandler2=signal(SIGTSTP,SIG_IGN);//ignore CTR-Z

for(int i=1; i<=10; i++){
printf("I am not sensitive to CTR-C/CTR-Z\n"); 
sleep(1);
}

signal(SIGINT, oldHandler1);	// restore default SIG_DFL
signal(SIGTSTP, oldHandler2); // restore default SIG_DFL

for(int i=1; i<=10; i++){
printf("I am sensitive to CTR-C/CTR-Z\n"); 
sleep(1);
}
}

