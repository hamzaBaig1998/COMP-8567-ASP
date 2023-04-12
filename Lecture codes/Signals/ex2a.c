#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(int argc, char *argv[]){
void (*oldHandler1)(); //to save default handlers 
void (*oldHandler2)(); //for CTR-C and CTR-Z
void (*newHandler1)(); //to save default handlers 
void (*newHandler2)(); //for CTR-C and CTR-Z

oldHandler1=signal(SIGINT,SIG_IGN); //ignore CTR-C 
oldHandler2=signal(SIGTSTP,SIG_IGN);//ignore CTR-Z

for(int i=1; i<=10; i++){
printf("I am not sensitive to CTR-C/CTR-Z\n"); 
sleep(1);
}

newHandler1=signal(SIGINT, oldHandler1);  // restore default 
newHandler2=signal(SIGTSTP,oldHandler2); // restore default

for(int i=1; i<=10; i++){
printf("I am sensitive to CTR-C/CTR-Z\n"); 
sleep(1);
}

oldHandler1=signal(SIGINT,newHandler1); //ignore CTR-C 
oldHandler2=signal(SIGTSTP,newHandler2);//ignore CTR-Z

for(int i=1; i<=10; i++){
printf("I am not sensitive to CTR-C/CTR-Z\n"); 
sleep(1);
}


}

