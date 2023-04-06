#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Passing a pointer to the thread function

void* func(void* p) {
   
   printf("From the thread function func\n");
   sleep(1);
   char * p1="\nReturned successfuly from Section 4 ";
      pthread_exit(p1);
 
}


main() {
   pthread_t t1; // declare thread
   void *ret;
   pthread_create(&t1, NULL, func, NULL);
   
   pthread_join(t1,&ret);
   
   
   
   printf("The return value fromm the thread is \n %s",ret); 
  
   
   } 
