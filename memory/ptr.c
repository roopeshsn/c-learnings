#include<stdio.h>

// NOTES:
// 1. Void pointers
// 2. Function pointers

void swap(int*, int*);
void printArray(int*, int);

int main() {
    int num = 10;
    int *ptr = &num;
    int **dptr = &ptr;
    printf("Variable num: %d \n", num);
    printf("Address of num: %x \n", ptr);
    printf("Address of ptr: %x \n", dptr);
    printf("Address of num: %x \n", &num);
    printf("Address of num+1: %x \n", &num+1);

    // Swap
    int a = 10, b = 20;
    printf("Value of a before swapping: %d \n", a);
    printf("Value of b before swapping: %d \n", b);
    swap(&a, &b);
    printf("Value of a after swapping: %d \n", a);
    printf("Value of b after swapping: %d \n", b);

    int *pb = &b;
    *pb = 30;
    printf("Value of a: %d \n", a);
    printf("Value of b: %d \n", b);

    // Print an array using Pointers
    int arr[5] = {10, 20, 30, 40, 50};
    printArray(&arr, 5);

    return 0;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int *arr, int n) {
    printf("Printing Array... \n");
    for(int i = 0; i < n; i++) {
        printf("Value at index %d: %d \n", i, *(arr+i));
    }
}