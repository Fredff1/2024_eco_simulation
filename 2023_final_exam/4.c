#include <stdio.h>
#define N 100
void Delete(char *str) {
    // 补充实现
    char* slow,*fast;
    int count=0;
    slow=str;
    fast=str+1;
    while((*fast)!='\0'){
        count=0;
        char*current;
        for(current=str;current<=slow;current++){
            if(*current==*fast){
                count++;
            }
            if(count==2){
                break;
            }
        }
        if(current==slow+1){
            *current=*fast;
            slow++;
        }
        fast++;

    }
    slow+=1;
    *slow='\0';
}
int main() {
    char str[N] = "abaccbadcda";
    Delete(str);
    printf("%s\n", str);
    return 0;
}