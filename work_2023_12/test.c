#include <stdio.h>


//int a[3];
//int b[3];
//int c[3][4];
//int*q=&c[0];
//int*p;
//int i=3;
//int j;
int *s,r;
int* s,r;
//两者等价，s为指针，r为整数

int mean(int a,int b){
    return (a+b)/2;
}

typedef int (*process_1)(int , int) ;


int main(){
    process_1 fun_1;
    fun_1=mean;
    for(int i=0;i<20;i++){
        printf("%d ",fun_1(i,i+10));
    }
    //a;
    //&a[0];
    //*a;
    //a++;
    //b=a;
   // p=a;
    //&(i+j);
    //*p=5;
    return 0;
}