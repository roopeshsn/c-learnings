#include <stdio.h>

union rectangle
{
    // the elements share same memory location. The size of the unio datatype will be the maximum size of a data member. In union the data member which is declared in the last will be printed correctly. 
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