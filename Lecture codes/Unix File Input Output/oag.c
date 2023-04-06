#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
printf("\nO_RDONLY  %d",O_RDONLY);
printf("\nO_WRONLY  %d",O_WRONLY);
printf("\nO_RDWR  %d",O_RDWR);
printf("\nO_APPEND  %d",O_APPEND);
printf("\nO_CREAT  %d",O_CREAT);
printf("\nThe result of O_RDONLY|O_WRONLY is %d\n",O_RDWR|O_CREAT);

}
