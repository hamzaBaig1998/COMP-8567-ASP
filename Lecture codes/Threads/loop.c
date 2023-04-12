#include <pthread.h>
#include <stdio.h>

void* myThreadFunc (){
for(int i=0;i<=20;i++)
{
printf("First thread\n");

}
printf("Returning from the first thread\n");
return NULL;
}

void* myThreadFunc1 (){
for(int i=0;i<=20;i++)
{
printf("Second thread\n");

}
printf("Returning from the second thread\n");
return NULL;
}

int main (int argc, char *argv[]){
pthread_t threadId1,threadId2;
// Create a new thread to run myThreadFunc()
pthread_create(&threadId1,NULL, myThreadFunc,NULL);
pthread_create(&threadId2,NULL, myThreadFunc1,NULL);
sleep(2);
printf("This is the main thread\n");

return(0);
} // compile with -lpthread library link

