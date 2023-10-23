#include <stdlib.h>
#include <time.h>
int main() {
// 设置随机数种子
srand(time(NULL));
int count=0;
for(int i=0;i<1000000;i++){
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if(x*x+y*y<=1){
            count+=1;
        }
    }
    double pi=(double)(count)/1000000.0*4;
    printf("%lf",pi);
}