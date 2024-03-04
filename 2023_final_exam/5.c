#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#ifndef CONWAY_H
#define CONWAY_H
#endif
int main(){
    int define=0;
    printf("%d\n",define);
    int*p;
    p=&define;
    *p=114514;
    define=strlen("I");
    printf("%d\n",define);
    const char a[5]={'a','b','c'};
    define=strlen(a);
    printf("%d\n",define);
    int z=0x0011bc;
    printf("%08x",z);
    return 0;
}