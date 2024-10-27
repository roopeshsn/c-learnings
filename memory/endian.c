// NOTES:
// Big endian and little endian system
// Little endian: LSB is stored first
// Big endian: MSB is stored first
// MAC follows little endian system

# include <stdio.h>

int main() {
    // Union DS is used because all members share the same memory location.
    // By assigning a value to an integer and then accessing the same memory as an array of bytes, 
    // we can directly observe how the integer is stored in memory, byte by byte. 
    // This allows you to visualize how the bytes of the integer are arranged in memory.
    union data {
        int val;
        char arr[4];
    };

    union data dataObj;
    dataObj.val = 0xAABBCCDD;

    printf("Address of dataObj = %p\n", &dataObj);
    printf("Address of dataObj.val = %p\n", &dataObj.val);
    printf("Address of dataObj.arr = %p\n", &dataObj.arr);

    for(int i = 0; i < 4; i++) {
        printf("Value at address %p = 0x%x\n", &dataObj.arr[i], (unsigned char)(dataObj.arr[i]));
    }
}