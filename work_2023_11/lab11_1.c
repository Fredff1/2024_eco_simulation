#include <stdio.h>
#include<stdbool.h>
#define STACK_SIZE 100

int content[100];
int top=0;

void make_empty(){
    top=0;
}


void stack_overflow(){
        printf("StackOverflow!\n");
        exit(1);
}

void stack_underflow(){
        printf("StackUnderflow\n");
        exit(1);
}

bool is_empty(){
    return top==0;
}

bool is_full(){
    return top>=STACK_SIZE;
}

void push(int i){
    if(is_full()){
        stack_overflow();
    }else{
        content[top]=i;
        top+=1;
    }
}

int pop(){
    if(is_empty()){
        stack_underflow();
        return -1;
    }else{
        top-=1;
        return content[top];
    }
}



int main(int argc,char*argv[]){
    make_empty();
    printf("Enter a number\n");
    int input_num=0;
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
        push(i);
    }
    printf("Successful!\n");
    printf("Enter a number\n");
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
        pop();
    }
    printf("Successful!\n");
}