#include <unistd.h>
#include <stdio.h>
 
 
 
 
int main(void) {
  char *programName = "ls";
  char *arg1 = "-1";

 
  execlp(programName, programName, arg1, NULL);
 
  return 0;
}
