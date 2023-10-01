#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int rows, cols;

void read_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("打开失败\n");
        return;
    }

    // 读取行数和列数
    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        printf("读取行数和列数失败\n");
        return;
    }
    printf("%d %d",rows,cols);
    int world[rows][cols];

    // 跳过第一行剩下的内容和换行符
    fscanf(fp, "\n");

    // 读取矩阵
    char ch;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ch = fgetc(fp);
            if (ch == '0') {
                world[i][j] = 0;
            } else if (ch == '1') {
                world[i][j] = 1;
            } else if (ch == '\n' || ch == EOF) {
                break;
            } else {
                
                return;
            }
        }
        fscanf(fp, "\n");  // 跳过换行符
    }
    for (int i=0;i<rows;++i){
        for (int j=0;j<cols;++j){
            printf("%d/n",world[i][j]);
        }
    }

    fclose(fp);
    
}

int main() {
    char filename[20];
    scanf("%s",filename);
    read_file(filename);
    printf("%d %d\n", rows, cols);
    
    return 0;
}
