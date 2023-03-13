#include <stdio.h>
#include <stdarg.h>

void add(int n, ... )
{
    va_list arg_list;    /* variable argument list variable */
    int x,sum=0;
    int arg;

    /* begin processing */
    va_start(arg_list,n);//Initializes arg_list to be used with va_arg and va_end macros
    //Stores the n arguments in arg_list
    
    printf("\nThe argument/s are\n");
    
    arg=va_arg(arg_list,int); //Extract all the arguments one-by-one (one per function call)
    sum=sum+arg;
    printf("\n%d",arg);
    
    arg=va_arg(arg_list,int); //Extract all the arguments one-by-one (one per function call)
    sum=sum+arg;
    printf("\n%d",arg); 
    
    arg=va_arg(arg_list,int); //Extract all the arguments one-by-one (one per function call)
    sum=sum+arg;
    printf("\n%d",arg); 
    
    printf("\nThe sum is %d\n",sum);
   
    va_end(arg_list); //Free up memory
}

int main()
{
    add( 3, 20, 10, 35);
    //add( 3, 40, 60, 10 );
    //add(10,1,2,3,4,5,6,7,8,9,10);

    return(0);
}
