#include<stdio.h>
#include<string.h>
int main(){
    char id[6];
    int door_number_input;
    char password_input[20];
    int number_input;
    int flag=0;//虽然但是，这个逻辑很奇怪啊，为什么只要满足一个条件就可以了
    printf("Enter your room number & card number & password\n");
    scanf("%d",&door_number_input);
    scanf("%d",&number_input);
    scanf("%s",&password_input);
    if(number_input==12345){
        flag=1;
    }else if(door_number_input==101||door_number_input==102||door_number_input==103){
        flag=1;
    }else if(strcmp(password_input,"guest")==0||strcmp(password_input,"password")==0||strcmp(password_input,"admin")==0){
        flag=1;
    }
    if(flag==1){
        printf("can pass!\n");
    }else{
        printf("request rejected\n!");
    }
}