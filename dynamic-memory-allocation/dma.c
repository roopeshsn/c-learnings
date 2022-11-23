#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dynamic Memory Allocation -> Memory will be allocated in runtime. Through DMA we can achieve efficient memory usage.

// malloc() -> allocate memory to user defined datatypes -> return void *
// calloc() -> allocate memory to derived datatypes
// realloc() -> increase or decrease the size
// free() -> delete the object in a memory

// pointer plays a major role in dynamic memory allocation

// 16-bit compiler int -> 2bytes
// 32-bit compiler int -> 4bytes

int main() {
    char *man;
    char nam[5];
    man = malloc(5);
    strcpy(man, "Roopesh");
    strcpy(nam, "Legend Roopesh");
    printf("%s", man);
    printf("%s", nam);
}