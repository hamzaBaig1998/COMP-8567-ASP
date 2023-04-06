#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[5];
int no_of_tickets=1;


//Without mutex
void* bookticket(void *arg)
{
   
   //Create a lock through mutex 
    if(no_of_tickets>=1){
    sleep(1);
    no_of_tickets=no_of_tickets-1;
    printf("Ticket booked to thread #: %d\n",pthread_self());
    } 
    else
    printf("Sorry,all tickets are booked\n");
    
    //Unlock the mutex 

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;


    while(i < 5)
    {
        err = pthread_create(&(tid[i]), NULL,bookticket, NULL);
        if (err != 0)
            printf("\ncan't create thread\n");
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
 

    printf("The main thread starts execution\n");
    return 0;
}
