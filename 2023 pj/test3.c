#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int rows, cols;

void read_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("��ʧ��\n");
        return;
    }

    // ��ȡ����������
    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        printf("��ȡ����������ʧ��\n");
        return;
    }
    printf("%d %d",rows,cols);
    int world[rows][cols];

    // ������һ��ʣ�µ����ݺͻ��з�
    fscanf(fp, "\n");

    // ��ȡ����
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
        fscanf(fp, "\n");  // �������з�
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
