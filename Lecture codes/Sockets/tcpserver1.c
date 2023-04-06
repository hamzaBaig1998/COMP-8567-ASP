#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h> 

  
int main(int argc, char *argv[]){

int sd, client, portNumber;
socklen_t len;
struct sockaddr_in servAdd;//Type IPV4

if(argc != 2){
fprintf(stderr,"Call model: %s <Port#>\n",argv[0]);
exit(0);
}

//socket()
if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Could not create socket\n");
exit(1);
}

servAdd.sin_family = AF_INET;
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);//Host to network long- byte order of IP address
sscanf(argv[1], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);//Host to network short- byte order of the port number 

//servAdd has the IP address and PORT address.
//bind

bind(sd, (struct sockaddr *) &servAdd,sizeof(servAdd));

//servAdd is a network address of type IPV4. It has to be cast into the generic network address type (struct sockaddr *)

//listen
listen(sd, 5);

while(1){
client=accept(sd,(struct sockaddr*)NULL,NULL);//accept()
if(client==-1) 
exit(1);
else 
printf("Client connection request was successfully accepted\n");

char * myMsg="Test Message from the server to the client: How are you doing?";
write(client, myMsg, 100);
close(client);
printf("Message sent to the client\n");
}

}
