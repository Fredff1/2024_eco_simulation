#include "conway.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>



// 其他的代码，包括main()函数等



// 其他的代码，包括main()函数等



void clear_screen() {
    // Use ANSI escape codes to clear the screen
    // system("cls");
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
}

void print_help() {}

void print_game(const Conway *c) {}

void *listen_break(void *flag) {
    char c = 0;
    while (c != 'b' && c != 't') {
        scanf(" %c", &c);
        scanf("%*[^\n]"); // 清除未读内容
    }

    *(int *)flag = 1;
    return NULL;
}

void automatic_evolve(Conway *c, int time) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break, &flag);
    while (flag != 1) {
        next_generation(c);
        clear_screen();
        print_game(c);
        print_conway(c);
        printf("automatically evolving...\n");
        usleep(time * 1000); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    clear_screen();
}

void *listen_break_special(void *flag) {
    char c = 0;
    while (c != 'b' && c != 't') {
        scanf(" %c", &c);
        scanf("%*[^\n]"); // 清除未读内容
    }
    *(int *)flag = 1;
    return NULL;
}

void automatic_evolve_special(Conway *c, int time) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break_special, &flag);
    while (flag != 1) {
        int number_of_producer = count_living_producer(c);
        int number_of_consumer = count_living_consumer(c);
        next_generation_special(c);
        c->grid_feature.count_turn += 1;
        clear_screen();
        print_conway_special(c);
        printf("Current producer:%d Current consumer:%d Current turn:%d\n", number_of_producer, number_of_consumer, c->grid_feature.count_turn);
        printf("automatically evolving...\n");
        usleep(time * 1000); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    clear_screen();
}

void print_game_conway_help(Conway *c) { // 集成打印游戏界面
    print_game(c);
    print_conway(c);
    print_help();
}

void print_game_conway_help_special(Conway *c) {
    print_game(c);
    print_conway_special(c);
    print_help();
}

int main() {

    

    Conway *c = update_conway(0, 0);
    c->probability = 50;
    srand(time(NULL));
    int flag_game = 1;
    int time = 1000;
    int flag_mode = Normal;
    char menu_command[100]; // 输入的指令
    char basic_command;     // 解析基础指令如b l c
    char filename[50];      // 获得打开/保存的文件名
    clear_screen();
    print_game(c);
    print_conway(c);
    print_help();
    while (flag_game) { // 主循环
        if (flag_mode == Normal) {
            printf("Current game mode:Normal Mode\nPlease enter your options!\n");
            fgets(menu_command, 100, stdin);
            clear_screen();
            sscanf(menu_command, "%c", &basic_command);
            switch (basic_command) { // i rows cols 情况
            case 'i': {
                sscanf(menu_command, "%c %d %d", &basic_command, &c->rows, &c->cols);
                if (c->rows <= 0 || c->cols <= 0 || c->rows > 300 || c->cols > 300) { // 输入负数会导致变为非常大的整数
                    c->rows = 0;
                    c->cols = 0;
                    delete_grids(c);
                    c = update_conway(0, 0);
                    print_game_conway_help(c);
                    printf("warning!Invalid input detected!Please try again\n"); // 错误时的返回值
                    break;
                }
                c = update_conway(c->rows, c->cols); // 初始化
                print_game_conway_help(c);
                break;
            }
            case 'n': { // 手动下一世代
                next_generation(c);
                print_game_conway_help(c);
                printf("Grids updated!\n");
                break;
            }
            case 'l': {                                                     // 导入文件
                sscanf(menu_command, "%c %48s", &basic_command, &filename); // 读取文件名
                c = new_conway_from_file(c, filename);
                if (c == NULL) {
                    c = update_conway(0, 0);
                    printf("Invalid filename!");
                }
                print_game_conway_help(c);
                break;
            }
            case 's': { // 保存文件
                filename[0] = '\0';
                sscanf(menu_command, "%c %48s", &basic_command, &filename); // 读取文件名
                save_conway(c, filename);
                printf("Grids saved as %s\n", filename);
                print_game_conway_help(c);
                break;
            }
            case 'c': { // 自动演化模式
                if (c->rows <= 0 || c->cols <= 0) {
                    c->rows = 0;
                    c->cols = 0;
                    print_game_conway_help(c);
                    printf("Invalid rows/cols detected!Failed to activate auto mode!\n");
                    break;
                }
                automatic_evolve(c, time);
                break;
            }
            case 'r': { // 随机初始化
                int temp_flag_random_set_living = sscanf(menu_command, "%c %d", &basic_command, &c->probability);
                if (temp_flag_random_set_living == 2 && c->probability > 0 && c->probability < 100 && c->rows > 0 && c->cols > 0) {
                    init_random(c);
                    print_game_conway_help(c);
                    printf("Successfully randomly set grids!\n");
                    break;

                } else {
                    c->probability = 50;
                    init_random(c);
                    print_game_conway_help(c);
                    printf("Warning:An error occurred when trying to set grids.The grids have been "
                           "set default!\n");
                }
                break;
            }
            case 'q': { // 退出游戏
                flag_game = 0;
                break;
            }
            case 't': { // 改变时间
                print_game_conway_help(c);
                printf("Please enter the pause time you want (Example:100ms)\n");
                int temp_flag;
                temp_flag = scanf("%d", &time);
                clear_input();
                if (temp_flag != 1) {
                    printf("Invalid time input!\nTime remain 1000ms\n");
                    time = 1000;
                    break;
                }
                break;
            }
            case 'h': { // 帮助
                print_help();
                break;
            }
            case 'p': { // 改变游戏模式
                // print_game_conway_help(c);
                printf("Change game mode to Special mode!\n");
                flag_mode = Special;
                delete_grids(c);
                c->rows = 0;
                c->cols = 0;
                break;
            }
            case 'b': {
                print_game_conway_help(c);
                printf("auto mode disabled");
                break;
            }
            default: { // 解析错误输入
                // clear_input();
                if (basic_command = '\n') {
                    print_game_conway_help(c);
                    break;
                }
                print_game_conway_help(c);
                printf("Invalid input!\n");
                break;
            }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } else if (flag_mode == Special) { // 到特殊模式
            printf("Current game mode:Special Mode\nPlease enter your options!\n");
            fgets(menu_command, 100, stdin);
            clear_screen();
            sscanf(menu_command, "%c", &basic_command);
            switch (basic_command) { // i rows cols 情况
            case 'i': {
                sscanf(menu_command, "%c %d %d", &basic_command, &c->rows, &c->cols);
                if (c->rows <= 0 || c->cols <= 0 || c->rows > 300 || c->cols > 300) { // 输入负数会导致变为非常大的整数
                    c->rows = 0;
                    c->cols = 0;
                    delete_special_grids(c);
                    c = new_conway_special(0, 0);
                    print_game_conway_help_special(c);
                    printf("Warning!Invalid input detected!Please try again\n\n"); // 错误时的返回值
                    break;
                }
                c = new_conway_special(c->rows, c->cols);
                print_game_conway_help_special(c);
                break;
            }
            case 'r': { // 随机生成
                int temp_flag_random_set_living = sscanf(menu_command, "%c %d", &basic_command, &c->probability);
                if (temp_flag_random_set_living == 2 && c->probability > 0 && c->probability < 100 && c->rows > 0 && c->cols > 0) {
                    init_random_special(c);
                    print_game_conway_help_special(c);
                    printf("Successfully randomly set grids!\n");
                    break;
                } else { // 一样的错误处理
                    c->probability = 50;
                    init_random_special(c);
                    print_game_conway_help_special(c);
                    printf("Warning:An error occurred when trying to set grids.The grids have been "
                           "set default!\n");
                    break;
                }
            }
            case 'p': {
                printf("Change game mode to Normal mode!\n");
                flag_mode = Normal;
                delete_special_grids(c); // 切换模式时会删除已有的格点
                c->rows = 0;
                c->cols = 0;
                break;
            }
            case 'n': {
                next_generation_special(c);
                int number_of_producer = count_living_producer(c);
                int number_of_consumer = count_living_consumer(c);
                c->grid_feature.count_turn += 1;
                print_game_conway_help_special(c);
                printf("Current producer:%d Current consumer:%d Current turn:%d\n", number_of_producer, number_of_consumer,
                       c->grid_feature.count_turn);
                printf("Current temperature:%d Current oxygen:%d Current light:%d\n", c->grid_feature.temperature, c->grid_feature.oxygen_resource,
                       c->grid_feature.light_resource);
                break;
            }
            case 'c': { // 自动演化模式
                if (c->rows <= 0 || c->cols <= 0) {
                    c->rows = 0;
                    c->cols = 0;
                    print_game_conway_help_special(c);
                    printf("Invalid rows/cols detected!Failed to activate auto mode!\n");
                    break;
                }
                automatic_evolve_special(c, time);
                break;
            }
            case 't': { // 改变时间
                print_game_conway_help_special(c);
                printf("Please enter the pause time you want (Example:100ms)\n");
                int temp_flag;
                temp_flag = scanf("%d", &time);
                clear_input();
                if (temp_flag != 1) {
                    printf("Invalid time input!\nTime remain 1000ms\n");
                    time = 1000;
                    break;
                }
                break;
            }
            case 'q': {
                // DrawGrid(c);
                break;
            }
            default: {
                if (basic_command = '\n') {
                    print_game_conway_help_special(c);
                    break;
                }
                print_game_conway_help_special(c);
                printf("Invalid input!\n");
                break;
            }
            }
        }
    }
    delete_grids(c);
    clear_screen();
    return 0;
}

