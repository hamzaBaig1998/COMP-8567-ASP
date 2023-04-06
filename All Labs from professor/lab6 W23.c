#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int parent_pid;
int asd = 0;
void int_handler(int sig)
{
    if (getpid() != parent_pid)
    {
        printf("Killed %d with ctr+c\n", getpid());

        kill(getpid(), SIGTERM);
    }
}
void cont_handler(int sig)
{
    printf("This is from process %d\n", getpid());
    kill(getpid(), SIGTSTP);
}
void stop_handler(int sig)
{
    sleep(4);
    asd++;
    if (asd <= 3)
    {
        kill(getpid(), SIGCONT);
    }
}

int main(int argc, char *argv[])
{
    int cou = 1;
    parent_pid = getpid();

    printf("First process, PID= %d, PPID= %d, PGID= %d\n", getpid(), getppid(), getpgid(0));

    signal(SIGINT, int_handler);
    signal(SIGCONT, cont_handler);
    signal(SIGTSTP, stop_handler);

    pid_t pid1, pid2, pid3;

    pid1 = fork();
    pid2 = fork();
    pid3 = fork();

    if (getppid() != parent_pid)
    {
        setpgid(0, parent_pid);
    }
    else
    {
        setpgid(0, getpid());
    }
    printf("PID=%d PPID= %d PGID= %d\n", getpid(), getppid(), getpgid(0));

    pause();

    if (pid1 != 0 && pid2 != 0 && pid3 == 0)
    {
        sleep(3);
        kill(getpid(), SIGCONT);
    }
    else if (pid1 != 0 && pid2 == 0 && pid3 != 0)
    {
        sleep(2);
        kill(getpid(), SIGCONT);
    }
    else if (pid1 == 0 && pid2 != 0 && pid3 != 0)
    {
        sleep(1);
        kill(getpid(), SIGCONT);
    }
    else if (pid1 != 0 && pid2 != 0 && pid3 != 0)
    {
        kill(getpid(), SIGCONT);
    }
}
