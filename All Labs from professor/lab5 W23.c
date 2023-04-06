#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int counter = 0;
time_t start_time;

void handler(int signum)
{
    time_t curr_time = time(NULL);
    if (curr_time - start_time > 5)
    {
        counter = 1;
        start_time = curr_time;
    }
    else
    {
        counter++;
    }
    if (counter == 2)
    {
        printf("Exiting\n");
        exit(0);
    }
}

int main(void)
{
    signal(SIGINT, handler);
    start_time = time(NULL);
    while (1)
    {
        printf("Welcome to Lab5 - Signals\n");
        sleep(1);
    }
    return 0;
}