#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//lseek2.c with SEEK_CUR

main()
{

int fd3=open("check.txt",O_RDWR);


char buff1[15];
for(int i=0;i<15;i++)
{
buff1[i]='A';
}

long int n=write(fd3,buff1,15); 
printf("\nThe value is n is %d\n",n);

//USING SEEK_CUR
n=lseek(fd3,10,SEEK_CUR);

printf("\nThe offset is %d\n",n);


for(int i=0;i<15;i++)
{
buff1[i]='B';
}

n=write(fd3,buff1,15); 

printf("\nThe number of bytes written from the offset is %ld\n",n);


close(fd3);

}
