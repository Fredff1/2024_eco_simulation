#include <stdio.h>
int a=5,b=10,c=15;
int main(){
    printf("%d\n",(1&&50));
    int d=a>b?a+c:b<c&&c%a==0?b*a:b-a;
    printf("%d",d);
}