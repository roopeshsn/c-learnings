#include<stdio.h>

int main() {
    int num = 10;
    int *ptr = &num;
    int **dptr = &ptr;
    printf("Variable num: %d \n", num);
    printf("Address of num: %d \n", ptr);
    printf("Address of ptr: %d \n", dptr);
    printf("Address of num: %d \n", &num);
    printf("Address of num+1: %d", &num+1);
    return 0;
}