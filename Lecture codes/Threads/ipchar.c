#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void* p) {
   
   printf("\nFrom the thread function\n");
   char *c=p; 
   printf("\nThe value of the input paramter to the thread function is %c\n",*c);
   sleep(1);
      
   return NULL;
}

main() {
   pthread_t t1; // declare thread
   char a='B';
   pthread_create(&t1, NULL, func, &a);
   sleep(2);
   printf("\nFrom the main function\n");
}
