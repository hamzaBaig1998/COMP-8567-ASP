#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int PROC_ELTIME;
int NO_OF_DFCS;

void terminateProcessTree(pid_t pid) {
    char cmd[256];
    sprintf(cmd, "ps --ppid %d -o pid=", pid);
    FILE *fp = popen(cmd, "r");
    char output[256];
    fgets(output, sizeof(output), fp);
    pclose(fp);
    char *token = strtok(output, " ");
    while (token != NULL) {
        int child_pid = atoi(token);
        printf("Checking child process with PID %d...\n", child_pid);
        sprintf(cmd, "ps --ppid %d -o stat=", child_pid);
        fp = popen(cmd, "r");
        fgets(output, sizeof(output), fp);
        pclose(fp);
        if (strncmp(output, "Z", 1) == 0) {
            printf("Child process with PID %d is defunct\n", child_pid);
            sprintf(cmd, "ps --ppid %d -o pid=", child_pid);
            fp = popen(cmd, "r");
            char child_output[256];
            fgets(child_output, sizeof(child_output), fp);
            pclose(fp);
            int no_of_dfcs = 0;
            char *child_token = strtok(child_output, " ");
            while (child_token != NULL) {
                int grandchild_pid = atoi(child_token);
                sprintf(cmd, "ps -o stat= -p %d", grandchild_pid);
                fp = popen(cmd, "r");
                fgets(output, sizeof(output), fp);
                pclose(fp);
                if (strncmp(output, "Z", 1) == 0) {
                    printf("Grandchild process with PID %d is defunct\n", grandchild_pid);
                    no_of_dfcs++;
                }
                child_token = strtok(NULL, " ");
            }
            if (no_of_dfcs > NO_OF_DFCS) {
                printf("Terminating process with PID %d (Has %d defunct child processes)\n", child_pid, no_of_dfcs);
                kill(child_pid, SIGKILL);
            } else {
                printf("Not terminating process with PID %d (Has %d defunct child processes)\n", child_pid, no_of_dfcs);
            }
        }
        terminateProcessTree(child_pid);
        token = strtok(NULL, " ");
    }
}

void terminateParent(pid_t pid) {
    pid_t ppid = getppid();
    time_t now = time(NULL);
    char cmd[256];
    sprintf(cmd, "ps -o etimes= -p %d", ppid);
    FILE *fp = popen(cmd, "r");
    char output[256];
    fgets(output, sizeof(output), fp);
    pclose(fp);
    int elapsed_time = atoi(output);
    if (ppid != 1 && ppid != pid && elapsed_time > PROC_ELTIME) {
        printf("Terminating parent process with PID %d (Elapsed Time: %d seconds)\n", ppid, elapsed_time);
        kill(ppid, SIGKILL);
    }
}

void searchDefunct(pid_t pid) {
    int status;
    pid_t child_pid = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
    if (child_pid == -1) {
        printf("Error: Failed to wait for child process with PID %d\n", pid);
        return;
    }
    if (child_pid == 0) {
        printf("No child process with PID %d is ready\n", pid);
        return;
    }
    if (WIFEXITED(status)) {
        printf("Child process with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
        terminateParent(pid);
    } else if (WIFSIGNALED(status)) {
        printf("Child process with PID %d terminated by signal %d\n", pid, WTERMSIG(status));
        terminateParent(pid);
    } else if (WIFSTOPPED(status)) {
        printf("Child process with PID %d stopped by signal %d\n", pid, WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("Child process with PID %d continued\n", pid);
    } else {
        printf("Unexpected status %d returned by waitpid()\n", status);
    }
    searchDefunct(pid);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [root_process] [OPTION1] [OPTION2]\n", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    if (pid == 0) {
        printf("Invalid PID\n");
        return 1;
    }
    if ((strcmp(argv[2], "-t") != 0) && (strcmp(argv[2], "-b") != 0))
    {
        printf("only options allowed are '-t' and '-b'");
        return 1;
    }

    int option3 = atoi(argv[3]);

    if (option3 < 1)
    {
        printf("Greater than 1 value is required\n");
        return 1;
    }
    printf("Searching for defunct processes in tree rooted at PID %d...\n", pid);

    if (strcmp(argv[2], "-t") == 0)
    {
        PROC_ELTIME = option3;
        searchDefunct(pid);
    } else {
        NO_OF_DFCS = option3;
        terminateProcessTree(pid);
    }
    
    return 0;
}
