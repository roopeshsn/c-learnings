#include <stdio.h>

union rectangle
{
    // the elements share same memory location
    int length;
    int width;
};

int main()
{
    union rectangle rect1;
    rect1.length = 5;
    printf("Length: %d \n", rect1.length);
    rect1.width = 10;
    printf("Length: %d, Width: %d \n", rect1.length, rect1.width);
    return 0;
}