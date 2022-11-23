#include <stdio.h>

int main() {
    // file handle
    FILE *fp;
    char str[100];
    fp = fopen("file.txt", "a"); // Read -> r, Write -> w, Append -> a, Read and Write -> r+

    // To read a string
    fgets(str, 6, fp);
    printf("%s", str);

    // To write a string
    fputs("Hello C!", fp);
    fputs("I am Roopesh", fp);

    // Good practice
    // Actually the write operation will store the content at whole in a buffer. Atlast when fclose() is called it write at that instance.
    fclose(fp);
}