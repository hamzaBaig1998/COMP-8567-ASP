//A client/server application(within a user) where a server
//accepts data from clients using the FIFO whose name is "server"
//Server program runs on one terminal 
//Client program/s run on other terminal/s 


#include <fcntl.h>
#include <stdio.h> // This is the server

int main(int argc, char *argv[]){
int fd;
char ch;

unlink("server"); // delete the FIFO file if it exists

if(mkfifo("server", 0777)!=0)//Create the FIFO file 
exit(1);

chmod("server", 0777);


printf("Waiting for a client\n");
fd = open("server", O_RDONLY);
printf("Got a client: ");
// The read call blocks until data is written to the pipe,
// until one end of the pipe is closed, 
// or the FIFO is no longer open for writing.
while(read(fd, &ch, 1) == 1) 
printf("%c", ch);

}//End main
