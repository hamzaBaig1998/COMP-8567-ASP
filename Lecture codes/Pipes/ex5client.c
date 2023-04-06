#include <fcntl.h> //Client
#include <stdio.h>

int main(int argc, char *argv[]){
int fd;
char ch;
int count=100;

while((fd=open("server", O_WRONLY))==-1)
{
printf("trying to connect to the server\n");
sleep(1);
}
printf("Connected: type in data to be sent\n");
while((ch=getchar()) != -1) // write all the input characters until -1( CTR-D )
write(fd, &ch, 1);

close(fd);
}
