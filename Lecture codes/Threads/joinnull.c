#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void* func(void* p) {
   
   printf("From the thread function func\n");
   sleep(1);
      pthread_exit(NULL);
      //returns NULL to the calling thread
}


main() {
   pthread_t t1,t2; 
   pthread_create(&t1, NULL, func, NULL);
   int ret=pthread_join(t1,NULL);
   printf("The return value is %d\n",ret); 
   
   } 
