#include <unistd.h>
 
int main(void) {
  char *programName = "ls";
  char *args[] = {programName, "-1", "/home/pranga/chapter2", NULL};
 
  execvp(programName, args);
 
  return 0;
}
