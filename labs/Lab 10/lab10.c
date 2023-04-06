#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 100

int ia1[ARRAY_SIZE], ia2[ARRAY_SIZE], ia3[ARRAY_SIZE];

void *sum_1(void *arg)
{
    int sum = 0;
    int *arr = (int *)arg;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sum += arr[i];
    }
    pthread_exit((void *)sum);
}

void *sum_2(void *arg)
{
    int sum = 0;
    int *arr = (int *)arg;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sum += arr[i];
    }
    pthread_exit((void *)sum);
}

void *sum_3(void *arg)
{
    int sum = 0;
    int *arr = (int *)arg;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sum += arr[i];
    }
    pthread_exit((void *)sum);
}

int main()
{
    pthread_t thread_1, thread_2, thread_3;
    void *thread_result;
    int sum1, sum2, sum3, total_sum;

    // Initialize the arrays with random integers between 10 and 20
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        ia1[i] = rand() % 11 + 10;
        ia2[i] = rand() % 11 + 10;
        ia3[i] = rand() % 11 + 10;
    }

    // Create threads and pass the arrays as arguments
    pthread_create(&thread_1, NULL, sum_1, (void *)ia1);
    pthread_create(&thread_2, NULL, sum_2, (void *)ia2);
    pthread_create(&thread_3, NULL, sum_3, (void *)ia3);

    // Wait for threads to finish and retrieve their return values
    pthread_join(thread_1, &thread_result);
    sum1 = (int)thread_result;
    pthread_join(thread_2, &thread_result);
    sum2 = (int)thread_result;
    pthread_join(thread_3, &thread_result);
    sum3 = (int)thread_result;

    // Calculate the total sum and print it
    total_sum = sum1 + sum2 + sum3;
    printf("Total sum: %d\n", total_sum);

    return 0;
}
