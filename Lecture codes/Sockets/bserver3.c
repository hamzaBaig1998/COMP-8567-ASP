#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>


int main(int argc, char *argv[]){
int sd, csd, portNumber, status;
socklen_t len;
struct sockaddr_in servAdd;//ipv4
if(argc != 2){
printf("Call model: %s <Port #>\n", argv[0]);
exit(0);
}
if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
printf("Cannot create socket\n");
exit(1);
}
//Add information to the servAdd struct variable of type sockaddr_in
servAdd.sin_family = AF_INET;
//When INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces.
//The htonl function takes a 32-bit number in host byte order and returns a 32-bit number in the network byte order used in TCP/IP networks
servAdd.sin_addr.s_addr = htonl(INADDR_ANY); //Host to network long 

sscanf(argv[1], "%d", &portNumber);
//uint16_t: optional integer type of exactly 16 bits
//htonos: Host to network short-byte order
servAdd.sin_port = htons((uint16_t)portNumber); 

//struct sockaddr is the generic structure to store socket addresses
//The procedure it to typecast the specific socket addreses of type sockaddr_in, sockaddr_in6, sockaddr_un into sockaddr

bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
listen(sd, 5);

while(1){
csd = accept(sd,(struct sockaddr *)NULL,NULL);
printf("Got a client\n");
if(!fork())//Child process 
child(csd);
close(csd);
waitpid(0, &status, WNOHANG); // waitpid? 
}
}//End main 


void child(int csd1){
char message[255];
int n, pid;
write(csd1, "start chatting", 16);

if(pid=fork()) /* reading csd messages */
{
while(1)
{
if(n=read(csd1, message, 255))
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
while(1)// sending messages csd
{
if(n=read(0, message, 255)){
message[n]='\0';
write(csd1, message, strlen(message));
if(!strcasecmp(message, "Bye\n")){
kill(getppid(), SIGTERM);
exit(0);
}
}
}//End while 
}
}//End child

