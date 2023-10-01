#include<stdio.h>
void switch1(int &a,int &b){
    int temp=a;
    a=b;
    b=temp;

}
int fenzu(int arr[],int low,int high){
    int tag=arr[high];
    int flag=low-1;
    for(int j=low;j<high;j++){
        if(arr[j]<tag){
            flag+=1;
            switch1(arr[flag],arr[j]);
        }
    }
    switch1(arr[flag+1],arr[high]);
    return flag+1;
}
void sort_value(int arr[],int low,int high){
    if (low<high){
        int tag1=fenzu( arr,low, high);

    
    sort_value(arr,low,tag1-1);
    sort_value(arr,tag1+1,high);
    }

}
int main(){
    int lenth;
    
    printf("请输入长度");
    scanf("%d",&lenth);
    int arr[lenth];
    for (int i=0;i<lenth;i++){
        printf("请输入元素4");
        scanf("%d",&arr[i]);

    }
    sort_value(arr,0,lenth-1);
    for (int p=0;p<lenth;p++){
        printf("%d ",arr[p]);
    }
    return 0;
}