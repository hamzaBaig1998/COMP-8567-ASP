#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h> 

//Server IP:137.207.82.53
  
int main(int argc, char *argv[]){
char message[100];
int server, portNumber;
socklen_t len;
struct sockaddr_in servAdd;

if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}

if ((server=socket(AF_INET,SOCK_STREAM,0))<0){ //socket()
fprintf(stderr, "Cannot create socket\n");
exit(1);
}

servAdd.sin_family = AF_INET; //Internet 
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);//Port number

//inet_pton() converts an Internet address in its standard text format into its numeric binary form
//Converting argv[1] into a numeric binary form and assigning it to servAdd.sin_addr
if(inet_pton(AF_INET, argv[1],&servAdd.sin_addr.s_addr) < 0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}

//connect()

if(connect(server, (struct sockaddr *) &servAdd,sizeof(servAdd))<0){//Connect()
fprintf(stderr, "connect() failed, exiting\n");
exit(3);
}

if (read(server, message, 100)<0){//read()
fprintf(stderr, "read() error\n");
exit(3);
}

fprintf(stderr, "%s\n", message);
exit(0);
}
