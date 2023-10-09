#include<stdio.h>
#include<math.h>
double nu,t,result;
int main(){
    scanf("%lf %lf",&nu,&t);
    if (nu>0&&t>0){
         result=(nu*nu*nu)/(exp(nu/t)-1);
         printf("%lf",result);
         return 0;
    }
}