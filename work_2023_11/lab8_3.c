#include <math.h>
#include <stdio.h>
#include <string.h>

#define HEIGHT 40
#define WIDTH 120
#define X_STEP 0.1
#define Y_STEP 0.05
#define PI 3.1415926
int main() {
    char canvas[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i += 1) {
        for (int j = 0; j < WIDTH; j += 1) {
            canvas[i][j] = ' ';
        }
    }

    for (double x = -5; x < 5; x += X_STEP) {
        double y;
        if (x - 0 < 1e-10&&x-0>-1e-10) {
            y = 1;
            continue;
        }
        y = (sin(PI * x)) / (PI * x);
        int i = HEIGHT / 2 - (int)(y /Y_STEP);
        int j = WIDTH / 2 + (int)(x /X_STEP);
        canvas[i][j] = '*';
    }

    for (int i = 0; i < HEIGHT; i += 1) {
        for (int j = 0; j < WIDTH; j += 1) {
            putchar(canvas[i][j]);
        }
        putchar('\n');
    }
}
