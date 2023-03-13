#include <unistd.h>
#include <stdio.h>
 
int main(void) {
  char *programName = "/bin/ls";
  char *arg1 = "-1";

 
  execl(programName, programName, arg1, NULL);
 
  return 0;
}
