#include <unistd.h>
 
int main(void) {
  char *programName = "/bin/ls";
  char *args[] = {programName, "-1", "/home/pranga/chapter2", NULL};
 
  execv(programName, args);
 
  return 0;
}
