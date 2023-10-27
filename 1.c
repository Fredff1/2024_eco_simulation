#include<stdio.h>
int n=9;
int main(){
int grid[10][10];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==0){
                grid[i][j]=j+1;
            }else{
                if(j==0){
                    grid[i][j]=grid[i-1][n-1];
                }else{
                    grid[i][j]=grid[i-1][j-1];
                }
            }
        }
    }

for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
        printf("%d",grid[i][j]);

    }
    printf("\n");
    }
}