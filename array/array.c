#include <stdio.h>

int main()
{
    int arr[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        printf("%d \n", arr[i]);
    }
    
    printf("%d \n", *arr);
    return 0;
}