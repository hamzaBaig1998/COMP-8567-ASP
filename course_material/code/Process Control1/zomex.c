#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char *argv[]){
int pid;
pid = fork();

if(pid==0)
{
//Child
printf("child process, pid=%d\n", getpid());
exit(0);
}
else if(pid<0)
{
printf("Error Forking"); 
}
else
{
//Parent
while(1)
sleep(5);
}


}
