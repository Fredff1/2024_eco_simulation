#include<stdio.h>
#include<string.h>
#include <stdlib.h>
int rows=0,cols=0;



void read_file(char *filename ){
    FILE *fp=fopen(filename,"r");
    if (fp==NULL){
        printf("打开失败");
    }
    int temp_cols=0;//读取列数，防止有不对齐的
    char ch;
     while ((ch = fgetc(fp)) != EOF) {
        if(ch!='\n'){
            temp_cols+=1;
       }else {
            rows+=1;
     }
         if (temp_cols<=6){
            temp_cols=0;
            rows-=1;
         }else {
            cols=temp_cols;
            temp_cols=0;
         }
     }
     int world[rows][cols];
     fseek(fp, 0, SEEK_SET);
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            ch=fgetc(fp);
            if(ch=='0'){
                world[i][j]=0;
            }else if(ch=='1'){
                world[i][j]=1;
            }else if(ch=='\n'||ch==EOF){
                break;
            }else{
                printf("文档不合格!");
            }
        }

    }

}
int main(){
    read_file("3.txt");
    printf("%d %d",rows,cols);
}