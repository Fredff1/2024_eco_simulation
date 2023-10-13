#include<stdio.h>
int main(){
//int a=0;
//scanf("%d",&a);
//if((a%4==0&&a%100!=0)||(a%400==0)){
    //printf("y\n");
//}else{
    //printf("n\n");
//}
int a;
printf("%d \n",(a=3*5,a*4,a+5));//,表示最后一个表达式的值
unsigned int light=0;
light=light|(1<<10);
int is_on=(light&(1<<10))>>10;
printf("%d",is_on);
}