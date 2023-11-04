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
    printf("i<row> <col>   initialize an empty grid with <row> rows and <col> columns\n");
    printf("r              randomly set alive/dead states for all grids\n");
    printf("n              evolve into next geberation\n");
    printf("c              automatically evolve, until receiving 'b' command\n");
    printf("b              pause evolution\n");
    printf("s <path>       save grid states to file <path>\n");
    printf("l <path>       load grid states from file <path>\n");
    printf("q              quit\n");
    printf("h              help\n");
}

void print_game( const Conway* c) {
    show_conway(c);
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
    uint16_t m,n;
    print_help();
    Conway c;
    c=new_conway(0,0);
    char running = 1;
    while (running) {
        char cmd;
        char path[40];
       // c=new_conway(0,0);你把这个写进循环是想让格点每次都被初始化是吧
        cmd=getchar();
       // printf("%c\n",cmd);
        switch (cmd)
        {
        case 'i':
            scanf("%hu%hu",&m,&n);
            //你这里错了，你定义了一个新的c,删除conway
            c=new_conway(m,n); 
            clear_screen();
            print_game(&c);
          //  show_conway(&c);       
            break;
        case 'c':
            automatic_evolve(&c);
            break;
        case 'h':
            print_help();
            break;
        case 'q':
            running=0;
            break;
        case 'r':
            init_random(&c);
            print_game(&c);
            break;
        case 'l':
            scanf("%s",path);
            c=new_conway_from_file(path);
            print_game(&c);
            break;
        case 's':
            scanf("%s",path);
            if(save_conway(&c,path)==1){
                printf("wrong path");
            }
            else{
                printf("successful save");
            }
            break;
        case 'n':
            next_generation(&c);    
            print_game(&c);
            break;
        default:
            break;
        }
    }
    return 0;
}