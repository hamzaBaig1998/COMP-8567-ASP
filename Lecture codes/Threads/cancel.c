// C program to demonstrates cancellation of another thread 
// using thread id
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
  
// To Count
int counter = 0; 
  

pthread_t tmp_thread2,tmp_thread3; 
  
// thread_one call func
void* func1(void* p) 
{
    while (1) {
  
        printf("thread number one\n");
        sleep(1);   // sleep 1 second
        counter++;   
        
        // for exiting if counter = = 5
        if (counter == 3) {
  
           
            pthread_cancel(tmp_thread2); 
            pthread_cancel(tmp_thread3);
            // for exit from thread_one 
            //pthread_exit(NULL);  
        }
    }
}
  

void* func2(void* p) 
{
  
    // store thread_two id to tmp_thread
    tmp_thread2 = pthread_self(); 
  
    while (1) {
        printf("thread Number two\n");
        sleep(1); // sleep 1 second
    }
}


void* func3(void* p) 
{
  
    // store thread_two id to tmp_thread
    tmp_thread3 = pthread_self(); 
  
    while (1) {
        printf("thread Number three\n");
        sleep(1); // sleep 1 second
    }
}



  

int main()
{
  
    // declare three threads
    pthread_t thread_one, thread_two,thread_three; 

    pthread_create(&thread_one, NULL, func1, NULL);
    
    pthread_create(&thread_two, NULL, func2, NULL);
     
    pthread_create(&thread_three, NULL, func3, NULL); 
 
    pthread_join(thread_one, NULL); 
    pthread_join(thread_two, NULL); 
    pthread_join(thread_three, NULL); 
    
  
}
