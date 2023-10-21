#include <stdio.h>
void change_into_binary(int x,int input1[4]){
    int flag;
    for(int i=0;i<4;i++){
        input1[3-i]=x%10;
        x/=10;
    }
    return;//转化成数组
}
int main(){
    int input_1,input_2;
    int input1[4]={0,0,0,0},input2[4]={0,0,0,0};
    int result[5]={0,0,0,0,0};
    int carry=0;
    printf("enter your number(example:1010-enter-1101-enter)\n");
    scanf("%d",&input_1);
    scanf("%d",&input_2);
    change_into_binary(input_1,input1);
    change_into_binary(input_2,input2);
    int bit1,bit2;
    for(int i=0;i<4;i++){
        bit1=input1[3-i];
        bit2=input2[3-i];
        int sum=(bit1^bit2)^carry;
        carry =(bit1&bit2)|(carry&(bit1^bit2));//如果进位为1，那么carry是1,如果进位是0但是上一个进位加上相加的值是1，那么carry还是1,其他情况进位为0
        result[4-i]=sum;
        
        //printf("%d\n",result[4-i]);
    
    }
    result[0]=carry;
    for(int i=0;i<4;i++){
        printf("%d",result[i+1]);
    }
     printf("\n%d",result[0]);
        
        
        
    
}