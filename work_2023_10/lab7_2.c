#include<stdio.h>
int main(){
    int n;
    scanf("%d",&n);
    int arr[2*n+1];
    for(int i=0;i<2*n;i++){
        if(i<n){
            arr[i]=i+1;
        }else{
            arr[i]=i+1-n;
        }
    }

    int pos=n;
    int arr_2[n][n];
    for(int i=0;i<n;i++){
        for(int j=pos;j<pos+n;j++){
            arr_2[i][j-pos]=arr[j];
        }
        pos-=1;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",arr_2[i][j]);
        }
        printf("\n");
    }

}