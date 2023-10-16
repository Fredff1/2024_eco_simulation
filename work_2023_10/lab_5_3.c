#include<stdio.h>
#include<string.h>
int main(){
    char id[6];
    int door_number_input;
    char password_input[20];
    int number_input;
    int flag=0;
    printf("Enter your room number & card number & password\n");
    scanf("%d",&door_number_input);
    scanf("%d",&number_input);
    scanf("%s",&password_input);
    if(number_input==12345){
        flag=1;
    }else if(door_number_input==101||door_number_input==102||door_number_input==103){
        flag=1;
    }else if(!(strcmp(password_input,"guest")&&strcmp(password_input,"password")&&strcmp(password_input,"admin"))){
        flag=1;
    }
    if(flag==1){
        printf("pass!");
    }else{
        printf("rejected!");
    }
}