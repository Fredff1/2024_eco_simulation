#include<stdio.h>
int a, b;
void swap1(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
} //????????
void swap2(int *a, int *b){
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
} //????
void swap3(int *a, int *b){
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
} //
int main(){
    printf("Enter your numbers: ");
    scanf("%d %d", &a, &b);
    swap1(&a,&b);
    swap2(&a,&b);
    swap3(&a,&b);
    printf(" %d %d\n", a, b);
    return 0;
}
