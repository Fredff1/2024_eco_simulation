#include<stdio.h>
#include<math.h>


int find_gcb(int a,int b){//欧几里得法寻找最大公因数
    while(b!=0){
        int temp=a%b;
        a=b;
        b=temp;
    }
    return a;
}

int main(){
    int n=11*17;
    int phi=(11-1)*(17-1);
    int e,d;
    for(int i=2;i<phi;i++){
        if(find_gcb(phi,i)==1){
            e=i;
            break;
        }
    }
    
    for(int i=1;i<phi;i++){
        if((i*e)%phi==1){
            d=i;
            break;
        }
    }
    int m=42;
    int m_1;
    int c;
    //加密
    int temp_x;
    temp_x=m;
    for(int i=0;i<e-1;i++){
        m=m*temp_x%n;
    }
    c=m%n;
    printf("加密后为%d\n",c);
    //解密
    temp_x=c;
    for(int i=0;i<d-1;i++){
        c=temp_x*c%n;//直接计算会越过最大值
    }
    m_1=c%n;
    printf("解密后为%d\n",m_1);
}