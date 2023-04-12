#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

int main(int argc, char *argv[])
{ 

int a=alarm(4);
printf("\nThe value is a is %d\n",a);

int b=alarm(10);
printf("\nThe value is b is %d\n",b);

while(1)
{
printf("Sleep for a second\n"); 
sleep(1);
}
printf("Exiting on Alarm\n"); 
exit(0);
}


