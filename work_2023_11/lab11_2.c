#include <stdio.h>
#include<stdbool.h>
#define STACK_SIZE 100



void make_empty(int *top){
    *top=0;
}


void stack_overflow(){
        printf("StackOverflow!\n");
        exit(1);
}

void stack_underflow(){
        printf("StackUnderflow\n");
        exit(1);
}

bool is_empty(int top){
    return top==0;
}

bool is_full(int top){
    return top>=STACK_SIZE;
}

void push(int i,int *top,int content[]){
    if(is_full(*top)){
        stack_overflow();
    }else{
        content[*top]=i;
        *top+=1;
    }
}

int pop(int *top,int content[]){
    if(is_empty(*top)){
        stack_underflow();
        return -1;
    }else{
        *top-=1;
        return content[*top];
    }
}



int main(int argc,char*argv[]){
    int top=0;
    int content[STACK_SIZE];
    make_empty(&top);
    printf("Enter a number\n");
    int input_num=0;
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
        push(i,&top,content);
    }
    printf("Successful!\n");
    printf("Enter a number\n");
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
        pop(&top,content);
    }
    printf("Successful!\n");
}