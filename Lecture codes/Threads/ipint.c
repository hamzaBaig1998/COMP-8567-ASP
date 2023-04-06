#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void* p) {
   
   printf("From the thread function\n");
   int *num;
   num=p; 
   printf("The value of the input paramter to the thread function is %d\n",*num);
   //printf("The value of the input paramter to the thread function is %d\n",(int)num);
   sleep(1);
      
   return NULL;
}



main() {
   pthread_t t1; // declare thread
   int a=3000;
   pthread_create(&t1, NULL, func, &a);
   sleep(2);
   printf("From the main function\n");
}
