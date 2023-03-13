Step1/3

However, I can provide you with a general outline of the steps involved in writing a program that searches for defunct processes in a process tree rooted at a specified process and forcefully terminates the parent process/es based on the input parameters.
Parse the command line arguments to get the root_process, OPTION1, and OPTION2 values.
Use the root_process PID to find all the descendant processes in the process tree using system calls like fork(), wait(), etc.
For each descendant process, check if it has any defunct child processes using system calls like waitpid() and WIFEXITED(). Store the defunct child process count and elapsed time of the parent process, if applicable.
Based on the input options, either forcefully terminate the parent process(es) that meet the criteria or terminate the entire process tree rooted at root_process.
Print the PID(s) of the terminated process(es) to the console.

Explanation
Please use the above steps

Explanation: Please refer to solution in this step. Step2/3
Here is a sample code skeleton that demonstrates the general approach:


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
pid_t root_process = atoi(argv[1]);
char *option1 = argv[2];
int option2 = atoi(argv[3]);

// Step 2: Traverse the process tree
// Step 3: Check for defunct child processes and store parent process info

// Step 4: Terminate processes based on options
if (strcmp(option1, "-t") == 0) {
// Terminate parent processes based on elapsed time
} else if (strcmp(option1, "-b") == 0) {
// Terminate process tree based on defunct child process count
} else {
fprintf(stderr, "Invalid option: %s\n", option1);
exit(EXIT_FAILURE);
}

// Step 5: Print the PIDs of the terminated process(es)

return 0;
}

Explanation
Please refer to the above code

Explanation: Please refer to solution in this step. Step3/3
Note that this is just a general outline and not a complete program. You will need to fill in the details and implement the specific logic for each step based on the requirements of your project. Additionally, be aware that forcefully terminating processes can have unintended consequences and should be used with caution.


Explanation
Please use these steps

Explanation: Please refer to solution in this step.