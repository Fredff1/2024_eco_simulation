#include<stdio.h>

long int fac(int n){  // 明确声明参数类型和函数返回类型5

    if(n <= 1){
        return 1;
    }
    return n * fac(n - 1);  // 使用半角分号
}

int main(){
    int n;
    do {
        scanf("%d", &n);
    } while(n <= 0);
    
    printf("%ld", fac(n));  // 使用 %ld 来格式化 long int
    return 0;
}
