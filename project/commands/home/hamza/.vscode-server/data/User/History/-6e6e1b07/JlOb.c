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

    struct person p1;
    strcpy(p1.name, "Hamza Baig");
    p1.age = 24;
    p1.dob.day = 1;
    p1.dob.month = 3;
    p1.dob.year = 1998;
}