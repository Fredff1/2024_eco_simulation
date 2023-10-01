#include<stdio.h>
void swap(int &a,int &b){
    int temp=a;
    a=b;
    b=temp;
}
int fenzu(int arr[],int low,int high){
    int tag=arr[high];
    int flag=low-1;
    for (int j=low;j<high;j++){
        if (arr[j]<tag){
            flag+=1;
            swap(arr[flag],arr[j]);

        }
    }
    swap(arr[flag+1],arr[high]);
    return flag+1;  
}

void sort_value(int arr[],int low,int high){
    if (low<high){
        int tag1=fenzu(arr,low,high);
        sort_value(arr,low,tag1-1);
        sort_value(arr,tag1+1,high);

    }

}
int main(){
    int lenth;
    scanf("%d",&lenth);
    int arr[lenth];
    for (int i=0;i<lenth;i++){
        scanf("%d",&arr[i]);
    }
    sort_value(arr,0,lenth-1);
    for (int r=0;r<lenth;r++){
        printf("%d ",arr[r]);
    }
    return 0;
}