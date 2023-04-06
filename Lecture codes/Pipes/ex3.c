#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//demonstrates dup2() system call
 
int main(void) {
  int number1, number2, sum;
 
 
  int input_fd = open("input.txt", O_RDONLY);//Enter two numbers in input.txt
  
  int output_fd = open("output.txt", O_RDWR);//Clear output.txt
  

  
  //Trying to redirect Standard Input (FD=0) to the file pointed by input_
  
  int ret1= dup2(input_fd,0); 
 
  if(ret1 < 0) {
    printf("Unable to duplicate the STDIN file descriptor.");
    exit(EXIT_FAILURE);
  }
  
   //Trying to redirect Standard Output (FD=1) to the file pointed by output_fd
   
   int ret2= dup2(output_fd, 1); 
 
  if(ret2 < 0) {
    printf("Unable to duplicate the STDOUT file descriptor.");
    exit(EXIT_FAILURE);
  }
  

   
   
  printf("The FD of input.txt is %d\n",input_fd);
  printf("The FD of output.txt is %d\n",output_fd);
  
  
  scanf("%d %d", &number1, &number2);
 
  sum = number1 + number2;
  
  printf("\nThe sum of two numbers is\n"); 
 
  printf("%d + %d = %d\n", number1, number2, sum);
 
  return EXIT_SUCCESS;
}
