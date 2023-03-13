#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PID_LEN 16

// function to check if a process is defunct
int is_defunct(char *pid)
{
    char stat_path[32];
    sprintf(stat_path, "/proc/%s/stat", pid);
    FILE *fp = fopen(stat_path, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return -1;
    }
    char stat_str[1024];
    fgets(stat_str, 1024, fp);
    fclose(fp);
    char *state = strtok(stat_str, " ");
    int i = 0;
    while (i++ < 2)
    {
        state = strtok(NULL, " ");
    }
    if (strcmp(state, "Z") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// function to recursively search for a process in the process tree
void search_process(char *root_pid, char *pid, int options)
{
    printf("Option: %d , root pid: %s \n", options, root_pid);
    char cmd_path[32];
    sprintf(cmd_path, "/proc/%s/cmdline", root_pid);
    FILE *fp = fopen(cmd_path, "r");
    if (fp == NULL)
    {
        return;
    }
    char cmd_str[1024];
    fgets(cmd_str, 1024, fp);
    fclose(fp);
    char *end = strchr(cmd_str, '\0');
    while (end > cmd_str)
    {
        end--;
        if (*end == '\0')
        {
            *end = ' ';
        }
    }
    char *end2 = strchr(cmd_str, ' ');
    if (end2 != NULL)
    {
        *end2 = '\0';
    }
    if (strcmp(cmd_str, "bash") != 0)
    {
        return;
    }
    char stat_path[32];
    sprintf(stat_path, "/proc/%s/stat", root_pid);
    fp = fopen(stat_path, "r");
    if (fp == NULL)
    {
        return;
    }
    char stat_str[1024];
    fgets(stat_str, 1024, fp);
    fclose(fp);
    char *pid_str = strtok(stat_str, " ");
    char *ppid_str = strtok(NULL, " ");
    if (strcmp(pid_str, pid) == 0)
    {
        printf("PID: %s PPID: %s\n", pid_str, ppid_str);
        if (options == 0)
        { // -c option
            char child_path[32];
            puts("-c option ");
            printf("root_pid:%s pid_str:%s\n", root_pid, pid_str);
            sprintf(child_path, "/proc/%s/task/%s/children", root_pid, pid_str);
            fp = fopen(child_path, "r");
            if (fp != NULL)
            {
                char child_pid_str[MAX_PID_LEN];
                while (fgets(child_pid_str, MAX_PID_LEN, fp) != NULL)
                {
                    printf("Child PID: %s", child_pid_str);
                }
                fclose(fp);
            }
        }
        if (options == 1)
        { // -s option
            char parent_path[32];
            sprintf(parent_path, "/proc/%s/status", ppid_str);
            fp = fopen(parent_path, "r");
            if (fp != NULL)
            {
                char parent_pid_str[MAX_PID_LEN];
                char *parent_ppid_str;
                while (fgets(parent_pid_str, MAX_PID_LEN, fp) != NULL)
                {
                    if (strncmp(parent_pid_str, "Pid:", 4) == 0)
                    {
                        parent_ppid_str = strtok(NULL, " ");
                        strtok(parent_pid_str, " ");
                        printf("Sibling PID: %s Sibling PPID: %s\n", parent_pid_str, parent_ppid_str);
                    }
                }
                fclose(fp);
            }
        }
        if (options == 2)
        { // -gp option
            char grandparent_path[32];
            sprintf(grandparent_path, "/proc/%s/status", ppid_str);
            fp = fopen(grandparent_path, "r");
            if (fp != NULL)
            {
                char grandparent_pid_str[MAX_PID_LEN];
                char *grandparent_ppid_str;
                while (fgets(grandparent_pid_str, MAX_PID_LEN, fp) != NULL)
                {
                    if (strncmp(grandparent_pid_str, "Pid:", 4) == 0)
                    {
                        grandparent_ppid_str = strtok(grandparent_pid_str, " ");
                        grandparent_ppid_str = strtok(NULL, " ");
                        printf("Grandparent PID: %s\n", grandparent_ppid_str);
                    }
                }
                fclose(fp);
            }
        }
        if (options == 3)
        { // -gc option
            char child_path[32];
            sprintf(child_path, "/proc/%s/task/%s/children", root_pid, pid_str);
            fp = fopen(child_path, "r");
            if (fp != NULL)
            {
                char child_pid_str[MAX_PID_LEN];
                while (fgets(child_pid_str, MAX_PID_LEN, fp) != NULL)
                {
                    search_process(child_pid_str, pid, 0x01);
                }
                fclose(fp);
            }
        }
        if (options == 4)
        { // -z option
            int defunct = is_defunct(pid_str);
            if (defunct == 1)
            {
                printf("Process is DEFUNCT\n");
            }
            else if (defunct == 0)
            {
                printf("Process is NOT DEFUNCT\n");
            }
        }
        if (options == 5)
        { // -zl option
            char child_path[32];
            sprintf(child_path, "/proc/%s/task/%s/children", root_pid, pid_str);
            fp = fopen(child_path, "r");
            if (fp != NULL)
            {
                char child_pid_str[MAX_PID_LEN];
                while (fgets(child_pid_str, MAX_PID_LEN, fp) != NULL)
                {
                    if (is_defunct(child_pid_str) == 1)
                    {
                        printf("Defunct Child PID: %s", child_pid_str);
                    }
                }
                fclose(fp);
            }
        }
    }
    else
    {
        char child_path[32];
        sprintf(child_path, "/proc/%s/task/%s/children", root_pid, pid_str);
        fp = fopen(child_path, "r");
        if (fp != NULL)
        {
            char child_pid_str[MAX_PID_LEN];
            while (fgets(child_pid_str, MAX_PID_LEN, fp) != NULL)
            {
                search_process(child_pid_str, pid, options);
            }
            fclose(fp);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [root_process] [process_id] [OPTION]\n", argv[0]);
        exit(1);
    }
    char *root_pid = argv[1];
    char *pid = argv[2];
    int options = 0;
    if (argc == 4)
    {
        char *option_str = argv[3];
        // printf("options: %s\n", option_str);
        if (strcmp(option_str, "-c") == 0)
        {
            // puts('-c options');
            options = 0;
        }
        else if (strcmp(option_str, "-s") == 0)
        {
            options = 1;
        }
        else if (strcmp(option_str, "-gp") == 0)
        {
            options = 2;
        }
        else if (strcmp(option_str, "-gc") == 0)
        {
            options = 3;
        }
        else if (strcmp(option_str, "-z") == 0)
        {
            options = 4;
        }
        else if (strcmp(option_str, "-zl") == 0)
        {
            options = 5;
        }
        else
        {
            printf("Invalid option: %s\n", option_str);
            exit(1);
        }
    }
    search_process(root_pid, "", options);
    return 0;
}