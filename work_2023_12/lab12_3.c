#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int string_match(const char *str, const char *tar) {
    if(tar==""){
        return 0;
    }
    int len = strlen(str);
    int tarLen=strlen(tar);
    int pos = 0;
    int flag=0;
    for (int i = 0; i < len; i++) {
        if (str[pos] == tar[0]) {
            flag=0;
            
            for(int i=pos;i<pos+tarLen;i++){
                if(str[i]!=tar[i-pos]){
                    flag=1;
                    break;
                }
            }
            
            if(flag==0){
                return pos;
            }
        }
        pos++;
    }
    return -1;
}

int main() {
    if (string_match("abcd", "") != 0) {
        fputs("test 1 failed", stderr);
        exit(1);
    }
    if (string_match("abcd", "bcde") != -1) {
        fputs("test 2 failed", stderr);
        exit(1);
    }
    if (string_match("here is an simple example", "example") != 18) {
        fputs("test 3 failed", stderr);
        exit(1);
    }
    if (string_match("here is an simple exampl", "example") != -1) {
        fputs("test 4 failed", stderr);
        exit(1);
    }
    puts("success");
    return 0;
}