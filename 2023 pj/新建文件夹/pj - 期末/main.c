#include <stdio.h>
#include <pthread.h>
#include "conway.h"
#include <unistd.h>

void clear_screen() {
    // Use ANSI escape codes to clear the screen
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
}

void print_help() {

}

void print_game(const Conway* c) {

}

void* listen_break(void* flag) {
    char c = 0;
    while (c != 'b') {
        scanf(" %c", &c);
        scanf("%*[^\n]"); // 清除未读内容
    }

    *(int*)flag = 1;
    return NULL;
}

void automatic_evolve(Conway* c) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break, &flag);
    while (flag != 1) {
        next_generation(c);
        print_game(c);
        printf("automatically evolving...\n");
        sleep(1); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    print_game(c);
}

int main() {
    Conway c = new_conway(0, 0);
    print_game(&c);

    char running = 1;
    while (running) {
        char cmd;
        char path[40];
        scanf(" %c", &cmd);
        switch (cmd)
        {
        case 'c':
            automatic_evolve(&c);
            break;
        // others
        default:
            break;
        }
    }

    return 0;
}