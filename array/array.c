#include <stdio.h>

// macro definition
// define SIZE = 5

// NOTES
// - Size of an array must be compile time constant (2D array is an example)

int* getIntArray(); 
void printArr(int *arr, int size);
int sum(int arr[]);

int main()
{
    int *arr;
    arr = getIntArray();
    printArr(arr, 5);
    int arr[5];
    printf("Enter 5 values: ");
    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }
    int ans = sum(arr);
    printf("%d", sum);
    return 0;
}

int* getIntArray() {
    static int arr[5];
    printf("Enter 5 values: ");
    for(int i = 0; i < 5; i++) {
        scanf("\n %d", &arr[i]);
    }
    return arr;
}

void printArr(int *arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d", * (arr+i));
    }
}

int sum(int arr[]) {
    int sum = 0;
    for(int i = 0; i < 5; i++) {
        sum += arr[i];
    }
    return sum;
}
