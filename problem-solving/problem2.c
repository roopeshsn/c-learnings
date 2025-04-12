// Write a program to print the size of all data types

#include <stdio.h>

int main() {
    printf("sizeof int: %lu \n", sizeof(int));
    printf("sizeof float: %lu \n", sizeof(float));
    printf("sizeof double: %lu \n", sizeof(double));
    printf("sizeof char: %lu \n", sizeof(char));
    printf("sizeof long: %lu \n", sizeof(long));
    printf("sizeof void: %lu \n", sizeof(void));
    // printf("sizeof signed: %lu \n", sizeof(signed));
    // printf("sizeof unsigned: %lu \n", sizeof(unsigned));
    return 0;
}