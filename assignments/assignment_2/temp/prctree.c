#include <stdio.h>
#include <stdlib.h>
// #include <sys/syscall.h>
// #include <linux/sysctl.h>
#include <sys/sysctl.h>
#include <string.h>
#include <unistd.h>

void searchProcess(int root_pid, int pid)
{
    // configurations
    int conf[4];
    size_t len;
    int ppid, status;

    // set the configurations for sysctl
    conf[0] = CTL_KERN;
    conf[1] = KERN_PROC;
    conf[2] = KERN_PROC_PID;
    conf[3] = root_pid;

    // get the process information for the root process
    struct kinfo_proc kp;
    len = sizeof(kp);
    if (sysctl(conf, 4, &kp, &len, NULL, 0) == -1)
    {
        perror("sysctl");
        return 1;
    }

    // check if the root process is a descendant of the current BASH process
    if (kp.kp_proc.p_pid != getpid())
    {
        puts("Error: Root process is not a descendant of the current bash.");
        return 1;
    }

    // set the conf for sysctl to search for the target process
    conf[3] = pid;

    // get the process information for the target process
    len = sizeof(kp);
    if (sysctl(conf, 4, &kp, &len, NULL, 0) == -1)
    {
        perror("sysctl");
        return 1;
    }

    // check if the target process is a descendant of the root process
    if (kp.kp_eproc.e_ppid != root_pid)
    {
        printf("Process %d is not a descendant of process %d.\n", pid, root_pid);
        return 1;
    }

    // print the PID and PPID of the target process
    printf("PID: %d, PPID: %d\n", kp.kp_proc.p_pid, kp.kp_eproc.e_ppid);

    // process command line arguments
    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            // list the PIDs of all the child processes
            conf[3] = kp.kp_proc.p_pid;
            len = sizeof(kp);
            while (sysctl(conf, 4, &kp, &len, NULL, 0) != -1 && kp.kp_eproc.e_ppid == pid)
            {
                printf("Child PID: %d\n", kp.kp_proc.p_pid);
                len = sizeof(kp);
            }
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            // list the PID and PPID of all the sibling processes
            conf[3] = kp.kp_eproc.e_ppid;
            len = sizeof(kp);
            while (sysctl(conf, 4, &kp, &len, NULL, 0) != -1)
            {
                if (kp.kp_proc.p_pid != pid)
                {
                    printf("Sibling PID: %d, PPID: %d\n", kp.kp_proc.p_pid, kp.kp_eproc.e_ppid);
                }
                len = sizeof(kp);
            }
        }
        else if (strcmp(argv[i], "-gp") == 0)
        {
            // list the PID of the grandparent process
            conf[3] = kp.kp_eproc.e_ppid;
            len = sizeof(kp);
            if (sysctl(conf, 4, &kp, &len, NULL, 0) != -1)
            {
                printf("Grandparent PID: %d\n", kp.kp_eproc.e_ppid);
            }
        }
        else if (strcmp(argv[i], "-gc") == 0)
        {
            // list the PIDs and PPIDs of all the grandchildren processes
            conf[3] = kp.kp_proc.p_pid;
            len = sizeof(kp);
            while (sysctl(conf, 4, &kp, &len, NULL, 0) != -1)
            {
                int child_pid = kp.kp_proc.p_pid;
                len = sizeof(kp);
                while (sysctl(conf, 4, &kp, &len, NULL, 0) != -1 && kp.kp_eproc.e_ppid == child_pid)
                {
                    printf("Grandchild PID: %d, PPID: %d\n", kp.kp_proc.p_pid, kp.kp_eproc.e_ppid);
                    len = sizeof(kp);
                }
                conf[3] = child_pid;
                len = sizeof(kp);
            }
        }
        else if (strcmp(argv[i], "-z") == 0)
        {
            // print the status of the target process
            conf[0] = CTL_KERN;
            conf[1] = KERN_PROC;
            conf[2] = KERN_PROC_PID;
            conf[3] = pid;
            len = sizeof(status);
            if (sysctl(conf, 4, &status, &len, NULL, 0) != -1)
            {
                if (status == 0)
                {
                    printf("Process %d is not defunct.\n", pid);
                }
                else
                {
                    printf("Process %d is defunct.\n", pid);
                }
            }
        }
        else if (strcmp(argv[i], "-zl") == 0)
        {
            // list the PIDs of all the defunct child processes
            conf[3] = kp.kp_proc.p_pid;
            len = sizeof(kp);
            while (sysctl(conf, 4, &kp, &len, NULL, 0) != -1 && kp.kp_eproc.e_ppid == pid)
            {
                int child_pid = kp.kp_proc.p_pid;
                conf[3] = child_pid;
                len = sizeof(status);
                if (sysctl(conf, 4, &status, &len, NULL, 0) != -1 && status != 0)
                {
                    printf("Defunct child PID: %d\n", child_pid);
                }
                conf[3] = pid;
                len = sizeof(kp);
            }
        }
        else
        {
            printf("Invalid option: %s\n", argv[i]);
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [root_process] [process_id] [OPTION]\n", arg[0]);
        return 1;
    }

    int root_pid = atoi(argv[1]);
    int pid = atoi(argv[2]);

    return 0;
}
