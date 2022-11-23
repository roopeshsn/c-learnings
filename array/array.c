#include <stdio.h>

// macro definition
// define SIZE = 5

int main()
{
    int *ans;
    ans = getIntArray();
    printArr(ans, 5);
    return 0;
}

int* getIntArray() {
    static int arr[5];
    printf("Enter 5 values: ");
    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }
    return arr;
}

void printArr(int *arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d", * (arr+i));
    }
}