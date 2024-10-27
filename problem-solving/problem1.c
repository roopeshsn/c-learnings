#include <stdio.h>

int main() {
    if(sizeof(int) > -1) {
        printf("True \n");
    } else {
        printf("False \n");
    }
    printf("sizeof(int): %lu \n", sizeof(int));
}