#include<stdio.h>

int main(int argc,char*argv[]){
    int hour,min,sec;
    while(scanf("%d::%d::%d",&hour,&min,&sec)!=EOF){
        int sum_min=60*hour+min;
        if(sum_min>=1410||sum_min<=450){
            printf("Sleep\n");
        }else{
            printf("wake up\n");
        }
    }
    return 0;

}