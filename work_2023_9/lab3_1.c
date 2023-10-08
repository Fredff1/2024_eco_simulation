#include<stdio.h>
//我不想打太多中文
int main(){
    char sign;
    int a,b,answer;
    int flag=0;
    
    printf("Please enter your calculating signs:");
    scanf("%c",&sign);
    printf("Please enter your numbers:");
    scanf("%d %d",&a,&b);
    if(sign=='+'){
        answer=a+b;
    }else if(sign=='-'){
        answer=a-b;
    }else if (sign=='*'){
        answer=a*b;
    }else if(sign=='/'){
        answer=a/b;
    }else{
        printf("Invailed sign!");
        flag=1;
    }
    if(flag==0){
    printf ("%d%c%d=%d",a,sign,b,answer);
    }
    return 0;    
}