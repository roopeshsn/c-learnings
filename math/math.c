#include <stdio.h>

int *maxMin(int *arr);
void print2dArr(int *arr, int size);

void main() {
    int arr[] = {1, 2, 3, 4, 5};
    int * a;
    a = maxMin(arr);
    printf("Max: %d \n", a[0]);
    printf("Min: %d \n", a[1]);
    int arr1[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int arr2[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int sum[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            sum[i][j] = arr1[i][j] + arr2[i][j];
        }
    }
    print2dArr(sum, 3);
    
    return 0;
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

void print2dArr(int *arr, int size) {
    /* The pointer arr is pointing to the first element of
    the array and the size is the size of the array. In the
    loop we are incrementing pointer so that it points to
    the next element of the array on each increment. */
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%d", arr);
        }
        arr++;
    }
}
