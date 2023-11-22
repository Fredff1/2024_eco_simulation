#include <stdio.h>
#include <stdlib.h>
#define EMPTY '-'
#define BLACK '1'
#define WHITE '0'
// 利用好这个函数打印棋子
// 除去run()里面的那次调用外，你可以只在一行用到这个函数
void print_pieces(char a[], int n_total) {
    for (int i = 0; i < 2 * n_total + 4; i += 1) {
        printf("%c", a[i]);
    }
    printf("\n");
}
void move_to(char a[], int n_total, int ori, int target) {
    int len = 2 * n_total + 4;
    int legal = 0;
    if (a[target] == '-') {
        legal = 1;
    }
    // TODO:
    // 给`legal`赋值：什么样的移动是不合法的？
    if (!legal) {
        printf("illegal move\n");
        exit(1);
    }
    a[target] = a[ori];
    a[target + 1] = a[ori + 1];
    a[ori] = '-';
    a[ori + 1] = '-';
    print_pieces(a, n_total);
    // TODO: 将ori处的棋子移动到i
    // 将j1处的棋子也移动过来, 保证其和ori棋子的相对位置不变
    //连续的棋子，j1是不必要的吧，我给删了
}

void move_pieces(char a[], int n_total, int n_current) {
    if (n_total < 3 || n_current < 3) {
        printf("invalid n\n");
        exit(1);
    }

    int n_treated;

    // printf("%d\n",n_treated);
    //printf("%d\n", n_current);
    if (n_current == 3) {
        move_to(a, n_total, 6 - 1, 3 - 1);
        move_to(a, n_total, 9 - 1, 6 - 1);
        move_to(a, n_total, 7 - 1, 1 - 1);
        //为了不被下标搞晕特意这么写
    } else {
        move_pieces(a, n_total, n_current - 1);
        n_treated = n_current - 1;
        move_to(a, n_total, (n_current + 1) * 2, (n_treated) * 2);
        move_to(a, n_total, (n_treated - 1), n_treated * 2 + 2);
        move_to(a, n_total, (n_treated + 1) * 2 - 1, n_treated - 1);
        move_to(a, n_total, n_treated + 1, n_treated * 2 + 1);
        move_to(a, n_total, (n_treated + 1) * 2, n_treated + 1);
    }
    // TODO
    // 当前关注的棋子对数n_current等于3时应该如何处理
    // n_current大于3对时应该如何处理
}
void run(int n) {
    int len = 2 * n + 4;
    char a[2 * n + 4];
    int is_black = 1;
    for (int i = 0; i < len; i += 1) {
        if (i < 4) {
            a[i] = EMPTY;
        } else {
            a[i] = is_black ? BLACK : WHITE;
            is_black = (!is_black);
        }
    }
    print_pieces(a, n);
    move_pieces(a, n, n);
}

int main() {
    printf("Please enter how many white chess you want\n");
    int num=0;
    scanf("%d",&num);
    run(num);
    return 0;
}
