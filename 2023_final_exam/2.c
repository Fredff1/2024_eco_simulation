#include <stdio.h>

int main() {
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // p 是一个指向含有4个整数的数组的指针
    int (*p)[4];

    // 使 p 指向 arr 的第一行
    p = arr;

    // 访问 p 所指向的数组的元素
    for (int i = 0; i < 4; i++) {
        printf("%d ", (*p+1)[i]);
    }

    return 0;
}
