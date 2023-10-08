#include<stdio.h>
int main(){
    float a,b;
    a=123456.789e5;
    b=a+20;
    if(abs(a-b)<1e-10){
        printf("y");
    }else{
        printf("n");
    }
    return 0;
}