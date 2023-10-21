#include<stdio.h>
#include<stdlib.h>
int main(){
    srand(time(NULL));
    unsigned int seed=rand()%101;
    unsigned int input;
    while(1){
        printf("enter your number\n");
        scanf("%d",&input);
        if(input==seed){
            printf("You are right\n");
            break;
        }
        printf("Incorrect\n");
    }


}