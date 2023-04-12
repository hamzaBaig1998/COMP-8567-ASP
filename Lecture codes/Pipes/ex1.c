// C program to illustrate
// pipe system call in C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/signal.h> 
#define MSGSIZE 200
char* msg1 = "Welcome to COMP8567 Advanced Systems Programming\n";


void pipehandler(int n)
{

printf("The SIGPIPE signal was caught\n");
exit(1);
} 

//As a simple example, the same process writes and reads from the pipe 
int main()
{
    char buf[200]=;
    int fd[2], i;
    char * msg="COMP8567-Advanced Systems Programming"; 
    
    if (pipe(fd) < 0) 
        exit(1);

     close(fd[0]);
        
     int ret=write(fd[1], msg, 40);
     
    int n= read(fd[0], buf, 45);
    
  
    return 0;
}
