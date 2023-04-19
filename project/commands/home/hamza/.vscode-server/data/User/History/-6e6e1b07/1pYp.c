#include <stdio.h>

void main()
{
    struct date
    {
        int day;
        int month;
        int year;
    };
    struct person
    {
        char name[64];
        int age;
        struct date dob;
    };
}