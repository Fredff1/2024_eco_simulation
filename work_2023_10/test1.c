#include<string.h>
#include<stdio.h>
int myAtoi(char * s){
    char temp;
    int result=0;
    int flag=1;
    int length=strlen(s);
    for(int i=0;i<length;i++){
        temp=s[i];
        if(temp==' '){
            if(result>0||(i>1&&s[i-1]==0)){
                break;
            }
            continue;
        }else if('0'<=temp&&temp<='9'){
            if(flag==1&&(result>214748364||(result==214748364&&(int)temp-48>7))){
                
                return 2147483647;
                break;
            }else if(flag==-1&&(result>214748364||(result==214748364&&(int)temp-48>8))){
                return -2147483648;
        
                break;
            }
            
            result=result*10+((int)(temp)-48);
        }else if(temp=='0'&&result<=214748364){
            if(result==0){
                continue;
            }else{
                result*=10;
            }
        }else if(temp=='-'){
            if(i>0&&s[i-1]=='+'){
                return 0;
            }else if(i>0&&s[i-1]=='0'){
                return 0;
            }
            flag=-1;
        }else if(temp=='+'){
            if(flag==-1){
                return 0;
            }else if(i>0&&s[i-1]=='0'){
                return 0;
            }
            continue;
        }else{
            break;
        }
    }
    
    return result*flag;
}
int main(){
    char a[40]="+0 123";
    long int result=myAtoi(a);
    printf("%d",result);
}