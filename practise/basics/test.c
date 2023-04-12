#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Passing a pointer to the thread function

void *func(void *p)
{

    char *p1 = "\nHello form the threaad\n";
    pthread_exit(p1);
}

main()
{
    pthread_t t1; // declare thread
    char *ret;
    pthread_create(&t1, NULL, func, NULL);

    pthread_join(t1, &ret);

    printf("\n%s", ret);
}
