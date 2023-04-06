#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void* p) {
    
   printf("From the thread function, the id of the thread = %d\n", pthread_self()); //get current thread id
   sleep(2);
 
   return NULL;
}


main() {
   pthread_t t1,t2; // declare thread
   pthread_create(&t1, NULL, func, NULL);
   sleep(2);
     printf("The value of t1 after pthread_create() = %d\n", t1);
     printf("From the main function, the id of the main thread is = %d\n", pthread_self()); 
   
}
