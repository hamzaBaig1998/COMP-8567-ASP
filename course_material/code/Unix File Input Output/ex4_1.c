#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("\n%d", STDIN_FILENO);
    printf("\n%d", STDOUT_FILENO);
    printf("\n%d", STDERR_FILENO);
    printf("\n%d\n", FOPEN_MAX);
}
