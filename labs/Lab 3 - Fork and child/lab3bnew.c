#include <stdio.h>
#include <unistd.h>

int main()
{
    int first_process_id, second_process_id, third_process_id;

    first_process_id = fork();

    int main_process = getpid();

    second_process_id = fork();
    third_process_id = fork();

    if (first_process_id > 0)
    {
        // No need to print here
    }
    else
    {

        if (second_process_id > 0)
        {
            printf("Welcome to COMP 8567 from process: %d whose parent is: %d and main process id is :%d\n", getpid(), getppid(), main_process);
        }
        else
        {

            if (third_process_id > 0)
            {
                printf("Welcome to COMP 8567 from process: %d whose parent is: %d and main process id is :%d\n", getpid(), getppid(), main_process);
            }
            else
            {
                printf("Welcome to COMP 8567 from process: %d whose parent is: %d and main process id is :%d\n", getpid(), getppid(), main_process);
            }
        }
    }

    return 0;
}