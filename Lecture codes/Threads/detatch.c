#include<stdio.h> 
#include<stdlib.h> 
#include<pthread.h>


void *Func(void *arg){
while(1)
{

printf("From the thread \n");
}
pthread_exit (NULL);
}


int main(){
pthread_attr_t att;
pthread_t tid;
pthread_attr_init(&att);
pthread_attr_setdetachstate(&att,PTHREAD_CREATE_DETACHED);
//The other option is PTHREAD_CREATE_JOINED


pthread_create(&tid, &att, Func, NULL);


 int ret=pthread_join(tid, NULL);
 
printf("\nThe return value is %d\n",ret);
pthread_attr_destroy(&att);
printf("The calling thread does not wait for the called thread\n");
 

}

