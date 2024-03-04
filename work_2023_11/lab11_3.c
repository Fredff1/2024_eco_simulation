#include <stdio.h>
#include<stdbool.h>
#define QUEUE_SIZE 100
#define STATE_EMPTY -1

bool is_full(int tail,int head){
    if(tail+1==head||(tail==QUEUE_SIZE-1&&head==0)){
        return true;
    }
    return false;
    
}

bool is_empty(int tail,int head){
    return tail==head;
}


void dequeue(int*tail,int *head,int content[]){
    if(is_empty(*tail,*head)){
        printf("EMPTY\n");
        exit(1);
    }else{
        if(*head==QUEUE_SIZE-1){
            content[*head]=STATE_EMPTY;
            *head=0;
        }else{
            content[*head]=STATE_EMPTY;
            *head+=1;
        }
    }
}

void enqueue(int i,int *head,int*tail,int content[]){
    if(is_full(*tail,*head)){
        printf("FULL\n");
        exit(1);
    }else{
        if(*tail==QUEUE_SIZE-1){
            *tail=0;
        }else{
            *tail+=1;
        }
        content[*tail]=i;
    }
}

int main(int argc,char*argv[]){
    int content[QUEUE_SIZE];
    int tail=0;
    int head=0;
    printf("Enter a number\n");
    int input_num=0;
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
       enqueue(i,&head,&tail,content);
    }
    printf("Successful!\n");
    printf("Enter a number\n");
    scanf("%d",&input_num);
    for(int i=0;i<input_num;i++){
        dequeue(&tail,&head,content);
    }
    printf("Successful!\n");
}