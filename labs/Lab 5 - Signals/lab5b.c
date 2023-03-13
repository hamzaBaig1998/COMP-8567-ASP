#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Counter
int counter = 0;

// interrupt timer
time_t interrupt_timer = 0;

// Flags and counter
int counter_for_timer = 0;
int flag_for_first_interrupt = 0;

void signalHandler(int signal_number)
{
    time_t current_time = time(NULL);
    if (signal_number == SIGINT)
    {

        if (counter_for_timer <= 5 && flag_for_first_interrupt == 1)
        {
            // exit if first interrupt and current intterupt difference is 5 seconds or less
            exit(0);
        }
        else
        {
            // reset counter if first interrupt and current intterupt difference greater than 5 seconds
            counter = 0;
        }
        flag_for_first_interrupt = 1;
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
        if (flag_for_first_interrupt == 1)
        {
            counter_for_timer++;
        }
        if (counter_for_timer > 5)
        {
            counter_for_timer = 0;
            flag_for_first_interrupt = 0;
        }
        sleep(1);
    }
    return 0;
}