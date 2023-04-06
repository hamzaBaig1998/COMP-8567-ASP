#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h> 

//int main(int argc, char *argv[]){
char message[255];
int server, portNumber;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}
if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);
B. Boufama 25
Chapter IX: Sockets Examples : implentation of client/server application
if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}
if(connect(server,(struct sockaddr *)&servAdd,
sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}
B. Boufama 26
Chapter IX: Sockets Examples : implentation of client/server application
while(1){
if(read(server, message, 255)<0){
fprintf(stderr, "read() error\n");
exit(3);
}
fprintf(stderr, "Server's messgae: %s\n",message);
fprintf(stderr, "Enter the $ sign to quit or
a message for the server\n");
fgets(message, 254, stdin);
if(message[0] == '$'){
close(server);
exit(0);
}
write(server, message, strlen(message)+1);
}
}
  
int main(int argc, char *argv[]){
char message[255];
int server, portNumber;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}
if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);

if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}
if(connect(server,(struct sockaddr *)&servAdd,
sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}


while(1){
if(read(server, message, 255)<0){
fprintf(stderr, "read() error\n");
exit(3);
}
fprintf(stderr, "Server's messgae: %s\n",message);
fprintf(stderr, "Enter the $ sign to quit or a message for the server\n");
fgets(message, 254, stdin);
if(message[0] == '$'){
close(server);
exit(0);
}
write(server, message, strlen(message)+1);
}
}
