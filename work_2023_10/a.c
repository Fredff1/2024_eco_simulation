#include <stdio.h>
void change_into_binary(int x,int input1[4]){
    int flag;
    for(int i=0;i<4;i++){
        input1[3-i]=x%10;
        x/=10;
    }
    return;
}

int main() {
    int input_1, input_2;
    int input1[4], input2[4];
    int result[5] = {0, 0, 0, 0, 0};
    int carry = 0;
    
    scanf("%d", &input_1);
    scanf("%d", &input_2);
    change_into_binary(input_1, input1);
    change_into_binary(input_2, input2);
    
    for (int i = 3; i >= 0; i--) {
        int bit1 = input1[i];
        int bit2 = input2[i];
        
        int sum = (bit1 ^ bit2) ^ carry;
        carry = (bit1 & bit2) | (carry & (bit1 ^ bit2));
        
        result[i + 1] = sum;
    }
    result[0] = carry;  // 处理最后一个进位
    
    // 输出结果
    for (int i = 0; i < 5; i++) {
        printf("%d", result[i]);
    }
    printf("\n");
    
    return 0;
}
