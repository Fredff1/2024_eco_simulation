#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANVAS_WIDTH 100
#define CANVAS_HEIGHT 64
#define PI 3.141592
char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

typedef double (*function_ptr)(double);

void print_menu() {
    printf("Enter the function you want\n");
    printf("-1:quit 0:linear 1:sin 2:cos 3:constant\n");
};

double function_linear(double x) { return x * 0.1; }

double function_sin(double x) { return sin(x); }

double function_square(double x) { return cos(x); }

double function_constant(double x){
    return 0;
}

void draw_function(function_ptr tar_fun) {

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            canvas[i][j] = ' ';
        }
    }
    int y_1, x_1;
    for (double i = 0; i < (CANVAS_WIDTH / 10); i += 0.1) {
        x_1 = (int)(i * CANVAS_WIDTH / 10);
        y_1 = CANVAS_HEIGHT / 2 - (int)(tar_fun(i) * (CANVAS_HEIGHT / 2 ));
        if (x_1 >= 0 && x_1 < CANVAS_WIDTH && y_1 >= 0 && y_1 < CANVAS_HEIGHT) {
            canvas[y_1][x_1] = '*';
        }
    }
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            putchar(canvas[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    function_ptr fun_arr[] = {function_linear, function_sin, function_square,function_constant};
    int flag = 0;
    int running=1;
    while (running == 1) {
        print_menu();
        scanf("%d", &flag);
        switch (flag) {
        case -1:
            running = 0;
            break;
        case 0:
            draw_function(fun_arr[0]);
            break;
        case 1:
            draw_function(fun_arr[1]);
            break;
        case 2:
            draw_function(fun_arr[2]);
            break;
        case 3:
            draw_function(fun_arr[3]);
            break;
        default:
            printf("Invalid\n");
        };
    }
    return 0;
}