#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//append.c

main()
{

//Appends buff1 at the end of test9.txt

int fd3=open("test9.txt",O_RDWR|O_APPEND);
char buff1[500];
long int n;

for(int i=0;i<500;i++)
{
buff1[i]='K';
if(i%10==0)
buff1[i]='\n';
}

n=write(fd3,buff1,500); 
if(n<0)
printf("\nWrite unsuccessful\n");

close(fd3);

}
