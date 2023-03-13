#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Counter
int counter = 0;
// interrupt timer
time_t interrupt_timer = 0;

void signalHandler(int signal_number)
{
    time_t current_time = time(NULL);
    if (signal_number == SIGINT)
    {
        if (current_time - interrupt_timer <= 5)
        {
            // exit if first interrupt and current intterupt difference is 5 seconds or less
            exit(0);
        }
        else
        {
            // reset counter if first interrupt and current intterupt difference greater than 5 seconds
            counter = 0;
        }
        interrupt_timer = current_time;
        counter++;
    }
}

int main()
{
    // Signal Handler for Ctrl + Z
    signal(SIGINT, signalHandler);
    while (1)
    {
        printf("Welcome to Lab5 - Signals\n");
        sleep(1);
    }
    return 0;
}
