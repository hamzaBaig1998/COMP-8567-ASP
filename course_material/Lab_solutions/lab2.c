#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//lab2 

int main(void)
{

umask(0000);
int fd1=open("sample.txt",O_CREAT|O_RDWR,0777);
if(fd1==-1)
printf("\n The operation was not successful\n");
else
printf("\n The file descriptor is %d\n",fd1);

//write
char *buff1="Welcome to COMP 8567";
long int n;
n=write(fd1,buff1,20);
printf("\n\nThe number of bytes written were %ld\n",n);
close(fd1);

//append 
fd1=open("sample.txt",O_RDWR | O_APPEND);
if(fd1==-1)
printf("\n The operation was not successful\n");
else
printf("\n The file is open in  the append mode and descriptor is %d\n",fd1);

char *buff2="Advanced Systems Programming";
long int n1;
n1=write(fd1,buff2,28);
printf("\n\nThe number of bytes written were %ld\n",n1);
close(fd1);

//write after 5 bytes
fd1=open("sample.txt",O_RDWR);
n=lseek(fd1,5,SEEK_SET);
printf("\nThe resulting offset is %ld\n",n);
char * buff3="*****";
n=write(fd1,buff3,5);
printf("\nThe no of bytes written from the resulting offset is %ld\n",n);

//write 4 bytes after the prev op
n=lseek(fd1,4,SEEK_CUR);
printf("\nThe resulting offset is %ld\n",n);
n=write(fd1,buff3,5);
printf("\nThe no of bytes written from the resulting offset is %ld\n",n);

//write 10 bytes after the end 
n=lseek(fd1,10,SEEK_END);
printf("\nThe resulting offset is %ld\n",n);
n=write(fd1,buff3,5);
printf("\nThe no of bytes written from the resulting offset is %ld\n",n);


//replace NULL 
char * buff4="@@@@@@@@@@";
n=lseek(fd1,48,SEEK_SET);
printf("\nThe resulting offset is %ld\n",n);
n=write(fd1,buff4,10);
printf("\nThe no of bytes written from the resulting offset is %ld\n",n);
close(fd1);


//Print 

fd1=open("sample.txt",O_RDONLY);
char buf[10];
char buff[4];
long int n2;

n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buf,10);
printf("%s\n",buf);
n2=read(fd1,buff,4);
printf("%s\n",buff);

n1=lseek(fd1,0,SEEK_END);
printf("The size of the file is %ld\n", n1);
close(fd1);
}




