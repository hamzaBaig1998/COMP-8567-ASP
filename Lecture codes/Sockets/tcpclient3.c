#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>

int main(int argc, char *argv[]){//E.g. 3 Client
char message[255];
int sid, portNumber, pid, n;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 3){
printf("Call model: %s <IP> <Port #>\n", argv[0]);
exit(0);
}
if((sid=socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);

//The inet_pton() function converts an Internet address in its standard text format into its numeric binary form
if(inet_pton(AF_INET, argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}
if(connect(sid, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}

read(sid, message, 255);
fprintf(stderr, "Message Received from Server: %s\n", message);
pid=fork();

if(pid)//Parent
{
while(1) /* reading sid's messages */
{
if(n=read(sid, message, 255)){
message[n]='\0';
fprintf(stderr,"%s\n", message);
if(!strcasecmp(message, "Bye\n")){
kill(pid, SIGTERM);//pid=process id of the child
exit(0);

}
}
}
}
else 
{
while(1) // sending messages to sid
{
if(n=read(0, message, 255)){
message[n]='\0';
write(sid, message, strlen(message)+1);
if(!strcasecmp(message, "Bye\n")){
kill(getppid(), SIGTERM);//getppid()=process id of the parent
exit(0);
}
}
}
}
}//End main

