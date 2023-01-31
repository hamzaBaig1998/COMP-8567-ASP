
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    int *arr;

    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

    arr = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        printf("Enter element at index %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // printing using index
    printf("The elements of the array using index: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    // De-referencing
    printf("The elements of the array in reverse order are: ");
    for (int i = n - 1; i >= 0; i--)
    {
        printf("%d ", *(arr + i));
    }

    free(arr);

    return 0;
}