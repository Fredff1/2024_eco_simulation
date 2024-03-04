#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    //int *p[4];
    int(*p)[4];
    int a[5][4]={0};
    
    p=a;
    for(int i=0;i<4;i++){
        printf("%d\n",(*p)[i]);
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%d",p[i][j]);
        }
        putchar('\n');
    }

    return 0;
}