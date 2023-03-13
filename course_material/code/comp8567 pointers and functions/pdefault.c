#include <stdio.h>
#include <stdlib.h>

main()
{

int *p; 

for(int i=0;i<=10;i++)
{
*(p+i)=i;
printf("\n%d\n",*(p+i));
}

printf("\nThe elements of the array are\n");
for(int i=0;i<=10;i++)
{
printf("\n%d\n",p[i]);
}

}


