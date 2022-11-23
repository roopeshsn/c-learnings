#include <stdio.h>

int main() {
    int arr[5];
    printf("Enter 5 values: ");
    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }
    int ans = sum(arr);
    printf("%d", ans);
    return 0;
}

int sum(int arr[]) {
    int sum = 0;
    for(int i = 0; i < 5; i++) {
        sum += arr[i];
    }
    return sum;
}