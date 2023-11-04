#include<stdio.h>
int main(){
    int pos=0;
    int arr[10]={3,4,2,6,5,7,88,34,1,0};
    int min=arr[0];
    
    for(int j=0;j<10;j++){
        min=arr[j];
        pos=j;
        for(int i=j;i<10;i++){
            if(min>arr[i]){
                min=arr[i];
                pos=i;
            }
        }
        int temp=arr[j];
        arr[j]=arr[pos];
        arr[pos]=temp;
        
    }
    for(int i=0;i<10;i++){
        printf("%d ",arr[i]);
    }
}