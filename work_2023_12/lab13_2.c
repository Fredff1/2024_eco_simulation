#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 9
int grids[SIZE][SIZE] = {{5, 3, 0, 0, 7, 0, 0, 0, 0}, {6, 0, 0, 1, 9, 5, 0, 0, 0}, {0, 9, 8, 0, 0, 0, 0, 6, 0},
                         {8, 0, 0, 0, 6, 0, 0, 0, 3}, {4, 0, 0, 8, 0, 3, 0, 0, 1}, {7, 0, 0, 0, 2, 0, 0, 0, 6},
                         {0, 6, 0, 0, 0, 0, 2, 8, 0}, {0, 0, 0, 4, 1, 9, 0, 0, 5}, {0, 0, 0, 0, 8, 0, 0, 7, 9}};
int *rows[SIZE]; 
//其实这个行指针完全没有必要，另外你对数独的要求实在太苛刻了，没有接触过这个算法不太可能写的出来

bool is_row_valid() {
    for (int i = 0; i < SIZE; i++) {
        bool flags[9] = {false};
         for (int j = 0; j < SIZE; j++) {
            int num = rows[i][j];
            if (num != 0) {
                if (flags[num - 1]) {
                    return false;
                } else {
                    flags[num - 1] = true;
                }
            } 
        }
    }
    return true;
}

bool is_col_valid() {
    for (int i = 0; i < SIZE; i++) {
        bool flags[9] = {false};
        for (int j = 0; j < SIZE; j++) {
            int num = grids[j][i];
            if (num != 0) {
                if (flags[num - 1]) {
                    return false;
                } else {
                    flags[num - 1] = true;
                }
            } 
        }
    }
    return true;
}

bool is_each_box_valid(int rowBox, int colBox) {
    bool flags[9] = {false};
    int num = 0;
    for (int i = rowBox * 3; i < 3 + rowBox * 3; i++) {
        for (int j = colBox * 3; j < 3 + colBox * 3; j++) {
            num = grids[i][j];
            if (num != 0) {
                if (flags[num - 1]) {
                    return false;
                } else {
                    flags[num - 1] = true;
                }
            } 
        }
    }
    return true;
}

bool is_box_valid() {
    bool flagList[9] = {false};
    for (int rowBox = 0; rowBox < 3; rowBox++) {
        for (int colBox = 0; colBox < 3; colBox++) {
            flagList[rowBox * 3 + colBox] = is_each_box_valid(rowBox, colBox);
        }
    }
    for (int i = 0; i < 9; i++) {
        if (flagList[i] == false) {
            return false;
        }
    }
    return true;
}

bool solve_puzzle() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grids[row][col] == 0) {  
                for (int num = 1; num <= SIZE; num++) { 
                    grids[row][col] = num;
                    if (is_row_valid() && is_col_valid() && is_box_valid()) {
                        if (solve_puzzle()) {
                            return true;
                        }
                        grids[row][col] = 0;
                    } else {
                        grids[row][col] = 0; 
                    }
                }
                return false; 
            }
        }
    }
    return true; 
}




int main() {
    for (int i = 0; i < 9; i++) {
        rows[i] = grids[i];
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
           //grids[i][j] = 1;
            printf("%d ", grids[i][j]);
        }
        printf("\n");
    }
    putchar('\n');

    solve_puzzle();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
           // grids[i][j] = 1;
            printf("%d ", grids[i][j]);
        }
        printf("\n");
    }
   /* if (!is_row_valid()) {
        printf("1\n");
    }
    if (!is_col_valid()) {
        printf("2\n");
    }
    if (!is_box_valid()) {
        printf("3\n");
    }*/
}