#include "conway.h"
#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// 其他的代码，包括main()函数等

// 初始化SDL
Bool initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return False;
    }
    return True;
}

// 生成一个窗口
SDL_Window *init_window(int length, int width) {
    SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length, width, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    return window;
}

// 渲染窗口
SDL_Renderer *render_window(SDL_Window *window) { // 渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

void draw_normal_sdlGrids(SDL_Renderer *renderer, Conway *c) {
    int temp_cell_size = 30;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            SDL_Rect rect;
            rect.x = j * temp_cell_size + j * 5;
            rect.y = i * temp_cell_size + i * 5;
            rect.w = temp_cell_size;
            rect.h = temp_cell_size;
            if (get_state(c, i, j) == Dead) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
            } else if (get_state(c, i, j) == Alive) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer); // 更新屏幕
}



void show_normal_sdl(Conway *c) {
    initialize_sdl();
    SDL_Window *window = init_window(640, 480);
    SDL_Renderer *renderer = render_window(window);
    draw_normal_sdlGrids(renderer, c);
    SDL_Delay(1000);
    Bool flag=True;
    while (flag==True) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) { //退出事件
            if (e.type == SDL_QUIT) {
                flag=False;
                break;
            }
        }

        SDL_RenderClear(renderer); // 清除旧的内容

        next_generation(c); // 更新Conway游戏的状态

        draw_normal_sdlGrids(renderer,c);

        SDL_Delay(1000); // 等待1秒
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}





void *sdl_listen_break(void *flag) {
    char x = 'g';
    Bool flag_nextGeneration = False;
    while (x != 'q') {
        scanf(" %c", &x);
        scanf("%*[^\n]"); // 清除未读内容
    }
    *(Bool *)flag = False;
    return NULL;
}

void show_normal_sdlGrids(Conway *c) {
    int flag = True;
    pthread_t listener;
    pthread_create(&listener, NULL, sdl_listen_break, &flag); // 新线程运行sdl_listen_break函数
    initialize_sdl();
    SDL_Window *window = init_window(640, 480);
    SDL_Renderer *renderer = render_window(window);
    draw_normal_sdlGrids(renderer, c);
    while (flag == True) {
        
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) { // 这里添加事件处理
            if (e.type == SDL_QUIT) {
                flag = False;
                break;
            }
            // 你也可以添加其他事件的处理代码
        }
        

        SDL_RenderClear(renderer); // 清除旧的内容

        next_generation(c); // 更新Conway游戏的状态

        draw_normal_sdlGrids(renderer,c);

        SDL_Delay(1000); // 等待1秒
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    pthread_join(listener, NULL);
}

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

int main(int argc, char *argv[]) {

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
            case 'g': { // 测试绘图功能
                show_normal_sdl(c);//单线程          
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
            case 'g': { // 测试绘图功能
                initialize_sdl();
                SDL_Window *window = init_window(640, 480);
                SDL_Renderer *renderer = render_window(window);
                draw_normal_sdlGrids(renderer, c);
                SDL_Event e;
                Bool quit = False;

                while (!quit) {
                    while (SDL_PollEvent(&e) != 0) {
                        if (e.type == SDL_QUIT) {
                            quit = True;
                        }
                    }
                }
                sleep(5);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
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
