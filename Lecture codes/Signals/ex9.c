#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 

#include <stdio.h>
#include <stdlib.h> 
#include <sys/signal.h> 
//Changing process group id

void CTR_handler(){ //handler 
printf("Process %d received CTR-C, exiting\n", getpid());
exit(0);
}

int main(int argc, char *argv[])
{
int i, pid;
signal(SIGINT, CTR_handler); //register the handler 
if((pid=fork()) == 0) //Child Process
{
printf("Child PID %d, PGID before setpgid() = %d\n",getpid(),getpgid(0));
setpgid(0, getpid()); //child’s group id is set to child’s PID
printf("Child PID %d, PGID after setpgid() = %d\n",getpid(),getpgid(0));
}
else //Parent Process
{
printf("Parent PID= %d, PGID=%d\n", getpid(), getpgid(0));
}

//Runs in both parent and child 
for(int k=0;k<=10;k++){
printf("Process %d is still alive\n", getpid()); 
sleep(2);
}
} //End Main

