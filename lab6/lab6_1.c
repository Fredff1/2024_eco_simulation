#include <stdio.h>
#include <math.h>

typedef enum{
    False=0,
    True=1,
}Bool;

Bool is_prime(int x){
    int y=sqrt(x)+1;
    Bool flag=True;
    if(x<2){
        flag=False;
    }
    for(int i=2;i<y;i++){
        if(x%i==0){
            flag=False;
            break;
        }
    }
    return flag;
}

int main(){
    for(int i=1;i<10001;i++){
        if(is_prime(i)==True&&is_prime(i+2)==True){
            printf("(%d, %d)\n", i, i + 2);
        }
    }
    return 0;
}