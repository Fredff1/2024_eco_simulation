#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverseArray(int *array, int length) {
    int temp_arr[length];
    for (int i = 0; i < length; i++) {
        temp_arr[i] = array[i];
    }
    for (int i = 0; i < length; i++) {
        int pos = length - i - 1;
        array[pos] = temp_arr[i];
    }
}
int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int length = sizeof(arr) / sizeof(arr[0]);
    reverseArray(arr, length);
    for (int i = 0; i < length; i++) {
        if (arr[i] != length - i) {
            fputs("error", stderr);
            exit(1);
        }
    }
    printf("success\n");
}