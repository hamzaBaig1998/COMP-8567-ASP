#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void* p) {
   
   printf("\nFrom the thread function\n");
   char *msg;
   msg=p; 
   printf("\nThe value of the input paramter to the thread function is %s\n",msg);
   sleep(1);
      
   return NULL;
}

main() {
   pthread_t t1; // declare thread
   char *msg1="\nWelcome to COMP8567\n";
   pthread_create(&t1, NULL, func, msg1);
   sleep(2);
   printf("\nMessage from the main function\n");
}
