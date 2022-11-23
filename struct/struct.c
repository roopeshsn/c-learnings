#include <stdio.h>
#include <string.h>

// Linked list will be created using Structure
// global structure
struct Rectangle
{
    int length;
    int width;
};

struct Book
{
    int id;
    char name[100];
    char author[50];
    float price;
};

int main()
{
    // local struture
    struct Triangle
    {
        int length;
        int breadth;
    };
    struct Rectangle rect1 = {5, 10};
    struct Rectangle rect2 = {10, 15};
    struct Triangle tria1 = {15, 20};
    printf("Size %d \n", sizeof(struct Rectangle));
    printf("Length: %d, Width: %d \n", rect1.length, rect1.width);
    printf("Length: %d, Width: %d \n", rect2.length, rect2.width);
    printf("Length: %d, Breadth: %d \n", tria1.length, tria1.breadth);

    struct Book book1;
    book1.id = 1;
    book1.price = 10.50;
    strcpy(book1.name, "Atomic Habits");
    strcpy(book1.author, "James Clear");
    printf("Id: %d, Name: %s, Author: %s, Price: %f", book1.id, book1.name, book1.author, book1.price);

    return 0;
}  