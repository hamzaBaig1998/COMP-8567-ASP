#include <stdio.h>

#define UNIX 0
//#define LINUX 100

int main()
{
   #ifdef UNIX
   printf("UNIX specific function calls go here.\n");
   #endif
   
   #ifdef LINUX
   printf("LINUX specific function calls go here.\n");
   #endif

   printf("TechOnTheNet is over 10 years old.\n");

   return 0;
}
