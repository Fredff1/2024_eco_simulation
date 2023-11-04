#include<stdio.h>
//假设矩阵为3*3,其实可以用动态数组算更大的，但是我比较懒所以就算了。手造轮子确实费劲
//这道题不像初学者能独立完成的东西，上网学习后得到这么个玩意
//关于输入矩阵，请自行调整源代码

const float epsilon = 1e-6;

float A[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
float I[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

void clear_screen() {
    // Use ANSI escape codes to clear the screen
    // system("cls");
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
}


void marti_mult(float mat1[3][3],float mat2[3][3],float result[3][3]){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int q=0;q<3;q++){
                result[i][j]+=mat1[i][q]*mat2[q][j];
            }
        }
    }
}

void print_matrix(float mat[3][3]){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("%f ",mat[i][j]);
        }
        printf("\n");
    }
}

void row_operation(float A[3][3],float I[3][3],int row1,int row2,float mult_num){
    for(int i=0;i<3;i++){
        A[row1][i]-=A[row2][i]*mult_num;
        I[row1][i]-=I[row2][i]*mult_num;
    }
}

void inverse(float A[3][3],float I[3][3]){
    for(int i=0;i<3;i++){//这里使寻找主元 ,每次处理对角线元素
        if(A[i][i]<epsilon&&A[i][i]>-epsilon){//防止出现主元为0
            for(int row=i+1;row<3;row++){
                if(A[row][i]>epsilon||A[row][i]<-epsilon){
                    for(int col=0;col<3;col++) {
                        float tempA=A[i][col];
                        A[i][col]=A[row][col];
                        A[row][col]=tempA;
                        
                        float tempI=I[i][col];
                        I[i][col]=I[row][col];
                        I[row][col]=tempI;//在下面的行数找到主元后交换两行，保证主元的位置合理
                    }

                }
                break;
            }
        }
        if(A[i][i]==0){
            printf("Cannot find pivot\n");//经过上面的操作还没有找到主元，说明不可逆
            return;
            
        }
        float pivot=A[i][i];
        for(int j=0;j<3;j++){
            A[i][j]/=pivot;
            I[i][j]/=pivot;//把主元所在的行规范，即变主元为1
        }
        for(int row=0;row<3;row++){
            if(row==i){
                continue;//主元所在的行不能自己减自己
            }
            float multi_num=A[row][i];//用于消元，把与主元同一列的之下的元素变成0
            row_operation(A,I,row,i,multi_num);
        }
    }
    
}


int main(){

    //print_matrix(A);
    //printf("\n");
   // print_matrix(I);
    printf("\n");
//
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("Please enter your matrix,current position row:%d col:%d\n",i+1,j+1);
            scanf("%f",&A[i][j]);
            clear_screen();
            printf("Current A:\n");
            print_matrix(A);
            printf("\n");
            
        }
    }
    
    inverse(A, I);
    //print_matrix(A);
    //printf("\n");
    printf("Inverse matrix is:\n");
    print_matrix(I);
    return 0;
    
}