#include<stdio.h>
void hanoi(int disk,char ori,char med,char target){
    if(disk==1){
        printf("Move %d disk from %c to %c\n",disk,ori,target);
    }else{
        hanoi(disk-1,ori,target,med);
        printf("Move %d disk from %c to %c\n",disk,ori,target);
        hanoi(disk-1,med,ori,target);
    }
}
int main(int argc,char*argv[]){
    int disk=0;
    const char ori='A',mid='B',target='C';
    printf("Enter the number of disks\n");
    scanf("%d",&disk);
    if(disk>30){
        printf("Warning:you enter a large number which may cause overflow\nAuto set 30 disks\n");
        disk=30;
    }
    hanoi(disk,ori,mid,target);
    return 0;
}