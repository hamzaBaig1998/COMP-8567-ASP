#include <stdio.h>
#include <sys/times.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_PID 32768 // maximum PID value on Linux
#define MAX_TIME 60   // maximum elapsed time in minutes

void terminate_parents(int pid, int el_time);
void terminate_tree(int pid, int num_defunct_children);
int count_defunct_children(int pid);

void terminate_parents(int pid, int proc_eltime)
{
    int ppid = getppid();
    if (ppid <= 1)
    { // reached init
        return;
    }

    // check if the process is defunct
    int status;
    if (waitpid(pid, &status, WNOHANG) == 0)
    {
        // process is still running, move on to its children
        terminate_parents(ppid, proc_eltime);
        return;
    }

    // process is defunct, check elapsed time
    struct tms time;
    if (times(&time) == -1)
    {
        perror("times");
        exit(EXIT_FAILURE);
    }

    int elapsed_time = time.tms_cutime / CLOCKS_PER_SEC / 60;
    if (elapsed_time >= proc_eltime)
    {
        printf("Terminating parent process %d (elapsed time: %d minutes)\n", ppid, elapsed_time);
        kill(ppid, SIGTERM);
    }

    // move on to the next parent
    terminate_parents(ppid, proc_eltime);
}

int count_defunct_children(int pid)
{
    int num_defunct_children = 0;

    int child_pid;
    int status;
    while ((child_pid = waitpid(pid, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status) || WIFSIGNALED(status))
        {
            // child process has terminated
            continue;
        }

        if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGCHLD)
        {
            // child process has become defunct
            num_defunct_children++;
        }
    }

    if (child_pid == -1 && num_defunct_children == 0)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    return num_defunct_children;
}

void terminate_tree(int pid, int no_of_dfcs)
{
    int defunct_children = count_defunct_children(pid);
    if (defunct_children >= no_of_dfcs)
    {
        printf("Terminating process %d (defunct children: %d)\n", pid,
               defunct_children);
        kill(pid, SIGTERM);
    }
    else
    {
        // move on to the children
        int child_pid;
        while ((child_pid = waitpid(pid, NULL, WNOHANG)) > 0)
        {
            terminate_tree(child_pid, no_of_dfcs);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s [root_process] [OPTION_1] [OPTION_2]\n", argv[0]);
        return 0;
    }
    if (strcmp(argv[2], "-t") == 0)
    {
        int root_pid = atoi(argv[1]);
        int proc_eltime = atoi(argv[3]);

        // check whether PROC_ELTIME valid or not
        if (root_pid <= 0 || proc_eltime <= 0 || proc_eltime > MAX_TIME)
        {
            puts("Invalid arguments");
            return 0;
        }

        // kill root_pid if exists
        if (kill(root_pid, 0) != 0)
        {
            printf("Process %d does not exist.\n", root_pid);
            return 0;
        }

        terminate_parents(root_pid, proc_eltime);
    }
    if (strcmp(argv[2], "-b") == 0)
    {
        int root_pid = atoi(argv[1]);
        int no_of_dfcs = atoi(argv[3]);

        // check whether PROC_ELTIME valid or not
        if (root_pid <= 0 || no_of_dfcs <= 0)
        {
            puts("Invalid arguments.");
            return 0;
        }

        // kill root_pid if exists
        if (kill(root_pid, 0) != 0)
        {
            printf("Process %d does not exist.\n", root_pid);
            return 0;
        }

        terminate_tree(root_pid, no_of_dfcs);
    }

    return 0;
}
