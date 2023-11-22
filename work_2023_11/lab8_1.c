#include <math.h>
#include <stdio.h>
int arr[6][32];

int main() {
    int p=0;
    printf("%d",p++);
    int input;
    int pos = 0;
    int flag = 1;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 64; j++) {
            if (j & flag) {
                arr[i][pos] = j;
                pos += 1;
            }
        }
        pos = 0;
        flag = flag << 1;
    }
    /*for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%d ", arr[i][j]);
            if (arr[i][j] == input) {}
        }
        printf("\n");
    }*/
    input=0;
    while (input >= 0 && input < 64) {
        printf("Enter a number(0-63)\n");
        scanf("%d", &input);
        if (input < 0 || input > 63) {
            printf("Invalid Input\n");
            input=0;
        }else{
            break;
        }
    }

    int dec[7] = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 32; j++) {
            if (arr[i][j] == input) {
                dec[i] = 1;
            }
        }
    }
    for (int i = 0; i < 6; i++) {
        if (dec[i] == 1) {
            printf("We add card%d \n", i + 1);
        }
    }
}