#include <stdio.h>
#include <stdlib.h> 

int main(void)
{

 printf("The PGID is process 100 is %d\n",setpgid(getpid(),getpid())); 
 
} 
