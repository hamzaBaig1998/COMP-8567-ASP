#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(int argc, char *argv[])
{ 

//After 4 seconds, the SIGALRM signal is sent by the kernel to the process
//The default action is to quit

alarm(4);

while(1)
{
printf("Sleep for a second\n"); 
sleep(1);
}
printf("Exiting on Alarm\n"); 
exit(0);
}


