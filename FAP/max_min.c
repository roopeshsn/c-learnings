#include <stdio.h>

int *maxMin(int *arr);

void main() {
    int arr[] = {1, 2, 3, 4, 5};
    int * a;
    a = maxMin(arr);
    printf("Max: %d \n", a[0]);
    printf("Min: %d \n", a[1]);
}

int *maxMin(int *arr) {
    int max, min;
    static int ans[2];
    size_t size = sizeof(arr) / sizeof(arr[0]);
    max = arr[0];
    min = arr[0];
    for(int i = 0; i < size; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    ans[0] = max;
    ans[1] = min;
    return ans;
}



