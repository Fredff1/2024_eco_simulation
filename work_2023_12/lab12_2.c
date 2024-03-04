#include<string.h>
#include <stdio.h>

void change_cap(char* input){
    int len=strlen(input);
    for(int i=0;i<len;i++){
        if(input[i]>='A'&&input[i]<='Z'){
            input[i]+=('a'-'A');
        }
    }
}

int is_you(char* list){
    change_cap(list);
    if(strcmp("you",list)==0){
        return 1;
    }
    return 0;
}
int main(){
    int n=0;
    int count=0;
    char s[100];
    scanf("%d",&n);
    for(int i=0;i<n;i++){
         scanf("%s",&s);
         count+=is_you(s);
    }
    printf("cnt:%d\n",count);
    return 0;
   

}