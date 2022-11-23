#include <stdio.h>

int main() {
    int x = 5;
    int y = 10;
    printf("x: %d y: %d \n", x, y);
    printf("x: %d y: %d \n", &x, &y);
    swap2(&x, &y);
    printf("x: %d y: %d \n", x, y);
    printf("x: %d y: %d \n", &x, &y);
    return 0;
}

// No swapping occurs because of scope of the function. This is known as call by value
void swap(int first, int second) {
    int temp = first;
    first = second;
    second = temp;
}

// Here the address is passed. This is known as call by reference
void swap2(int *first, int *second) {
    int temp = *first;
    *first = *second;
    *second = temp;
}