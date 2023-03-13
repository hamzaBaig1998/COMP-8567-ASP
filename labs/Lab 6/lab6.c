#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int grand_child_terminated = 0;
int pid_1, pid_2, pid_3;
int grand_child_paused = 0;

void main_sig_termination_handler(int sig)
{
    if (grand_child_terminated == 0)
    {
        // terminate grand children using pgid.
        killpg(getpid(), SIGUSR1);
        kill(pid_1, SIGUSR1);
        kill(pid_2, SIGUSR1);
        kill(pid_3, SIGUSR1);
        grand_child_terminated = 1;
    }
}

void sig_handler(int sig)
{
    printf("Grandchildren process terminating with PID: %d || PPID %d\n ", getpid(), getppid());
    exit(0);
}

void grand_child_sig_flag(int sig)
{
    grand_child_paused = 1;
}

void im_sig_cont_handler(int sig)
{
    grand_child_paused = 1;
}

int main()
{
    int main_pid = getpid();

    // set pgid as its own pid.
    if (setpgid(getpid(), main_pid) == -1)
    {
        perror("setpgid");
        exit(1);
    }
    pid_1 = fork();
    pid_2 = fork();
    pid_3 = fork();
    if (pid_1 < 0 || pid_2 < 0 || pid_3 < 0)
    {
        printf("error in fork() call");
        exit(1);
    }
    if (getpid() != main_pid)
    {

        // ignore ctrl + C in children
        signal(SIGINT, SIG_IGN);
        // SIGUSR1 for interprocess communication
        signal(SIGUSR1, sig_handler);
        // group as of main process., remove immediate childs from that process group.
        if (getppid() == main_pid)
        {
            signal(SIGUSR1, grand_child_sig_flag);

            if (setpgid(getpid(), getpid()) == -1)
            {
                perror("setpgid");
                exit(1);
            }
        }
        while (1)
        {
            printf("This is from process %d\n", getpid());
            if (grand_child_paused == 0)
            {
                sleep(2);
            }
            else
            {
                sleep(20);
            }
        }
    }
    else
    {

        // parent process
        signal(SIGINT, main_sig_termination_handler);

        // ignore SIGUSR1 in parent
        signal(SIGUSR1, SIG_IGN);

        // ignore SIGCONT in parent
        signal(SIGCONT, SIG_IGN);

        sleep(2);
        while (1)
        {
            sleep(2);
            if (grand_child_terminated != 0)
            {
                // grandchildren will be terminated
                printf("This is from process %d\n", getpid());
                sleep(2);
                kill(pid_1, SIGCONT);
                sleep(2);
                kill(pid_2, SIGCONT);
                sleep(2);
                kill(pid_3, SIGCONT);
            }
        }
    }

    return 0;
}
