#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//byteswrite.c
main()
{
//Writes into check.txt from an array of characters
int fd3=open("check.txt",O_RDWR);
char buff1[500];
for(int i=0;i<500;i++)
{
buff1[i]='S';
if(i%10==0)
buff1[i]='\n';
}
long int n;
n=write(fd3,buff1,500); 
printf("\n\nThe number of bytes written into the file were %ld\n",n);
close(fd3);
}
