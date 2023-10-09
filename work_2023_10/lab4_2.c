#include<stdio.h>
char input;
int move_length;
int main(){
    scanf("%c %d",&input,&move_length);
    if(input-move_length<='Z'&&input-move_length>='A'||input-move_length<='z'&&input-move_length>='a'){
        printf("%c",input-move_length);
    }else{
        printf("%c",input-move_length+26);
    }
    return 0;
}