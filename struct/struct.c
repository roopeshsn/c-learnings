#include <stdio.h>

// global structure
struct rectangle
{
    int length;
    int width;
};

int main()
{
    // local struture
    struct triangle
    {
        int length;
        int breadth;
    };
    struct rectangle rect1 = {5, 10};
    struct rectangle rect2 = {10, 15};

    printf("Size %d \n", sizeof(struct rectangle));

    printf("Length: %d, Width: %d \n", rect1.length, rect1.width);
    printf("Length: %d, Width: %d \n", rect2.length, rect2.width);
    return 0;
}  