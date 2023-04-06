#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>


int main(int argc, char *argv[]){// E.g. 3: Server
int sd, client, portNumber, status;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 2){
printf("Call model: %s <Port #>\n", argv[0]);
exit(0);
}
if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
printf("Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
sscanf(argv[1], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);
bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
listen(sd, 5);

while(1){
client = accept(sd,(struct sockaddr *)NULL,NULL);
printf("Got a client\n");
if(!fork())//Child process 
child(client);
close(client);
waitpid(0, &status, WNOHANG); // waitpid? 
}
}//End main 


void child(int sd){
char message[255];
int n, pid;
write(sd, "start chatting", 16);

if(pid=fork()) /* reading client messages */
{
while(1)
{
if(n=read(sd, message, 255))
{
message[n]='\0';
fprintf(stderr,"%s\n", message);
if(!strcasecmp(message, "Bye\n")){
kill(pid, SIGTERM);
exit(0);
}
}
}
}
else
{
while(1)// sending messages client
{
if(n=read(0, message, 255)){
message[n]='\0';
write(sd, message, strlen(message)+1);
if(!strcasecmp(message, "Bye\n")){
kill(getppid(), SIGTERM);
exit(0);
}
}
}//End while 
}
}//End child

