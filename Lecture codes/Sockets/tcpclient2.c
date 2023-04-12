#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>

int main(int argc, char *argv[]){
char message[255];
int ssd, portNumber;
socklen_t len;
struct sockaddr_in servAdd;

if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}
if((ssd = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}

servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);//convert host bytestream into network bytestream

if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}


if(connect(ssd,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}



while(1){
if(read(ssd, message, 255)<0){
fprintf(stderr, "read() error\n");
exit(3);
}
fprintf(stderr, "server's message: %s\n",message);

fprintf(stderr, "Enter the $ sign to quit or a message for the server\n");
fgets(message, 254, stdin);
if(message[0] == '$'){
close(ssd);
exit(0);
}
write(ssd, message, strlen(message)+1);
}
}










