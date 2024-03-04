#include <stdio.h>
#include <pthread.h>
#include "conway.h"
#include <unistd.h>
#include <time.h>


void clear_screen() {
    // Use ANSI escape codes to clear the screen
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
}

void print_help() {
    printf("i <row> <col>   initialize an empty grid with <row> rows and <col> cols\n");
    printf("r               randomly set alive/dead states for all grids\n");
    printf("n               evolve into next generation\n");
    printf("c               automatically evolve, until receiving 'b' command\n");
    printf("b               pause evolution\n");
    printf("s <path>        save grid states to file <path>\n");
    printf("l <path>        load grid states from file <path>\n");
    printf("w               change mode\n");
    printf("q               quit\n");
    printf("h               help\n");
}

void print_help_special() {
    printf("i <row> <col>   initialize an empty grid with <row> rows and <col> cols\n");
    printf("r               randomly set alive/dead states for all grids\n");
    printf("n               evolve into next generation\n");
    printf("c               automatically evolve, until receiving 'b' command\n");
    printf("f               change Dead/Alive state of one cell\n");
    printf("b               pause evolution\n");
    printf("s <path>        save grid states to file <path>\n");
    printf("l <path>        load grid states from file <path>\n");
    printf("w               change mode\n");
    printf("q               quit\n");
    printf("h               help\n");
}

void print_game(Conway *c){
    show_conway(c);
}

void print_game_special(Conway *c){
    show_conway_special(c);
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
    Conway *c = new_conway(0, 0);
    print_game(c);
    clear_screen();
    print_help();
    srand(time(NULL));
    char running = 1;
    char running_mode = Normal;
    char cmd;
    char path[40];
    while (running) {   
        if (running_mode == Normal) {  
            printf("You are in Normal Mode!\nHave your choice!\n");
            scanf(" %c", &cmd);
            switch (cmd){
                case 'i': {
                    printf("Input two numbers in 1~100\n");
                    scanf("%hu %hu", &c->m, &c->n);
                    c->possibility = 50;
                    if(c->m < 0 || c->n < 0 || c->m >100 || c->n > 100) {
                        c->m = 0;
                        c->n = 0;
                        delete_grids(c);
                        c = new_conway(0, 0);
                        print_help();
                        printf("Wrong command\n");
                        log_message("An error occured","Warning");
                        break;
                    }
                    c = new_conway(c->m, c->n);
                    printf("An empty grid is set up!\n");
                    log_message("New grid!","INFO");
                    print_game(c);
                    print_help();
                    break;
                }

                case 'r': {  //随机地初始化格点
                    int random_set_living = 0;
                    random_set_living = scanf("%hu", &c->possibility);
                    if (random_set_living == 1 && c->possibility > 0 && c->possibility < 100 && c->m > 0 && c->n > 0) {
                        init_random(c);
                        print_game(c);
                        print_help();
                        printf("Successfully randomly set grids!\n");
                        log_message("Successfully initiated!","INFO");
                        break;
                    } 
                    else {
                        c->possibility = 50;
                        init_random(c);
                        print_game(c);
                        print_help();
                        printf("Warning:Wrong possibility!\n");
                        log_message("Wrong possibility","WARNING");
                    }
                    break;
                }

                case 'n': {  // 手动下一世代
                    next_generation(c);
                    print_game(c);
                    print_help();
                    printf("Grids updated!\n");
                    log_message("Grids updated!","INFO");
                    break;
                }

                case 'c': {
                    if (c->m <= 0 || c->n <= 0) {
                        c->m = 0;
                        c->n = 0;
                        print_help();
                        printf("Failure!Invalid rows/cols detected!\n");
                        log_message("An error occured","ERROR");
                        break;
                    }
                    automatic_evolve(c);
                    break;
                }

                case 'w': {
                    running_mode = Special;
                    log_message("Change mode","INFO");
                    delete_grids(c);
                    break;
                }

                case 'q': { // 退出游戏
                    running = 0;
                    printf("quit game\n");
                    break;
                }

                case 'h': { // 帮助
                    print_help();
                    break;
                }

                case 'b': {
                    print_help(c);
                    printf("Pause evolution!\n");
                    break;
                }

                case 's': {  //保存文件
                    path[0] = '\0';
                    scanf("%20s", &path);  //读取文件名
                    int save_flag;
                    save_flag = save_conway(c, path);
                    if (save_flag == 0) {
                        printf("Successfully saved as %s", path);
                    }
                    print_game(c);
                    print_help();
                    break;
                }

                case 'l': {  //读取文件
                    scanf("%20s", &path); // 读取文件名
                    c = new_conway_from_file(path);
                    if (c == NULL) {
                        c = new_conway(0, 0);
                        printf("Fail to load grids!");
                    }  
                    print_game(c);
                    print_help();
                    break;
                }

                default: {
                    break;
                }
            }
        }

        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////
        else if(running_mode == Special) {
            printf("Current game mode:Special mode!\nHave your choice!\n");
            clear_screen();
            scanf(" %c", &cmd);
            switch(cmd){
                case 'i': {
                    printf("Input two numbers in 1~100\n");
                    scanf("%hu %hu", &c->m, &c->n);
                    c->possibility = 50;
                    if(c->m < 0 || c->n < 0 || c->m >100 || c->n > 100) {
                        c->m = 0;
                        c->n = 0;
                        delete_grids(c);
                        c = new_conway(0, 0);
                        print_help_special();
                        printf("Wrong command\n");
                        log_message("An error occured","Warning");
                        break;
                    }
                    c = new_conway(c->m, c->n);
                    printf("An empty grid is set up!\n");
                    log_message("New grid in special mode!","INFO");
                    print_game_special(c);
                    print_help_special();
                    break;
                }
                
                case 'r': {
                    int random_set_living = 0;
                    random_set_living = scanf("%hu", &c->possibility);
                    if (random_set_living == 1 && c->possibility > 0 && c->possibility < 100 && c->m > 0 && c->n > 0) {
                        init_random(c);
                        print_game_special(c);
                        print_help_special();
                        printf("Successfully randomly set grids!\n");
                        log_message("Successfully initiated in special mode!","INFO");
                        break;
                    } 
                    else {
                        c->possibility = 50;
                        init_random(c);
                        print_game_special(c);
                        print_help_special();
                        printf("Warning:Wrong possibility!\n");
                        log_message("Wrong possibility","WARNING");
                    }
                    break;
                }

                case 'n': { // 手动下一世代
                    next_generation(c);
                    print_game_special(c);
                    print_help_special();
                    printf("Grids updated!\n");
                    break;
                }

                case 'c': {
                    if (c->m <= 0 || c->n <= 0) {
                        c->m = 0;
                        c->n = 0;
                        print_help_special();
                        printf("Failure!Invalid rows/cols detected!\n");
                        log_message("An error occured","ERROR");
                        break;
                    }
                    automatic_evolve(c);
                    break;
                }

                case 'f': {
                    int rows, cols;
                    printf("Choose a grid: 0~%hu(not include %hu) and 0~%hu(not include %hu)\n", c->m, c->m, c->n, c->n);
                    scanf("%d %d", &rows, &cols);
                    if(rows < 0 || cols < 0 || rows >= c->m || cols >= c->n){
                        printf("Wrong Input!\n");
                        print_help_special();
                        break;
                    }
                    else{
                        turn_state(c, rows, cols);
                        break;
                    }
                }

                case 'w': {
                    running_mode = Normal;
                    log_message("Change mode","INFO");
                    delete_grids(c);
                    break;    
                }

                case 'q': {  // 退出游戏
                    running = 0;
                    printf("quit game\n");
                    break;
                }

                case 'h': {  // 帮助
                    print_help_special();
                    break;
                }
                
                case 'b': {
                    print_help_special(c);
                    printf("Pause evolution!\n");
                    break;
                }

                case 's': {  //保存文件
                    path[0] = '\0';
                    scanf("%20s", &path);  //读取文件名
                    int save_flag;
                    save_flag = save_conway(c, path);
                    if (save_flag == 0) {
                        printf("Successfully saved as %s", path);
                    }
                    print_game(c);
                    print_help();
                    break;
                }

                case 'l': {  //读取文件
                    scanf("%20s", &path); // 读取文件名
                    c = new_conway_from_file_special(path);
                    if (c == NULL) {
                        c = new_conway(0, 0);
                        printf("Fail to load grids!");
                    }  
                    print_game(c);
                    print_help();
                    break;
                }

                default: {
                    break;
                }
            }
            
        }
    }
    delete_grids(c);
    clear_screen();
    return 0;
}