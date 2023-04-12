#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void child(int *);
void parent(int *);

int main(int argc, char *argv[]){
int fd[2];
int k=pipe(fd);//create a pipe
if(k == -1)
exit(1); 

if(fork() == 0)// Fork a child process. Child inherits fd[0] and fd[1] from the parent process
child(fd);
else
parent(fd);

exit(0);
}

void parent(int *fd){
char ch;
char buff1[255];
close(fd[1]);//since parent does not use fd[1]
printf("From the Parent process->Child has sent the following message:\n");

do{
read(fd[0], &ch, 1);
printf("%c", ch);
if(ch == '\n')
break;
}
while(1);
}

void child(int *fd){
close(fd[0]);//since child does not use fd[0]
char message[255]="Message from the child..\n";
write(fd[1], message, 26);
}
