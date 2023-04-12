#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void* p) {
   
   //printf("Message from the thread with thread id=%d\n",pthread_self());
   sleep(1);
      pthread_exit(NULL);
   return NULL;
}


main() {
   pthread_t t1,t2,t3,t4; // declare thread
   
   pthread_create(&t1, NULL, func, NULL);
   printf("\nFrom the main function with thread id %d\n",t1);
   pthread_create(&t1, NULL, func, NULL);
   printf("\nFrom the main function with thread id %d\n",t1);
   pthread_create(&t1, NULL, func, NULL);
   printf("\nFrom the main function with thread id %d\n",t1);
   pthread_create(&t1, NULL, func, NULL);
   printf("\nFrom the main function with thread id %d\n",t1);
   sleep(2);
   printf("\nFrom the main function with thread id %d\n",pthread_self());
}
