#include <stdio.h>
#include <stdbool.h>

// Notes:
// - String in C is a 1D char array but array of strings is an 2D char array

int stringLength(char string[]);
bool stringCompare(char first[], char second[]);

int main() {
    // Length
    char apple[] = "apple";
    int size = stringLength(apple);
    printf("\nSize of the string %s is %d", apple, size);

    // Compare
    char first[] = "apple";
    char second[] = "apple";
    bool isEqual = stringCompare(first, second);
    (isEqual) ? printf("\nfirst and second are equal!") : printf("\nfirst and second are not equal!");

    return 0;
}

int stringLength(char string[]) {
    int size = 0;
    while(string[size] != '\0') {
        size++;
    }
    return size;
}

bool stringCompare(char first[], char second[]) {
    if(stringLength(first) != stringLength(second)) {
        return false;
    }
    int i = 0;
    while(first[i] != '\0' || second[i] != '\0') {
        if(first[i] != second[i]) {
            return false;
        }
        i++;
    }
    return true;
}

// TODO:
// stringCopy (strcpy)
// stringConcat (strcat)
