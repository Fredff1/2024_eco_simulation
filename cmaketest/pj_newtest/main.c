#include "conway.h"
// #include <pthread.h>
// #include <unistd.h>
// 不需要我自己写多线程了

// 返回一个初始化的按钮
Button initializeButton() {
    Button newButton;
    newButton.color = (SDL_Color){0, 0, 0};
    newButton.isClicked = False;
    newButton.rect = (SDL_Rect){0};
    newButton.font = NULL;
    newButton.x_move = 0;
    newButton.y_move = 0;
    newButton.text = NULL;
    return newButton;
}

// 初始化sdl，失败返回false
Bool initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! S  DL_Error: %s\n", SDL_GetError());
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

// 渲染窗口的渲染器
SDL_Renderer *render_window(SDL_Window *window) { // 渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

// 封装一个创建按钮的函数
Button createButton(int x, int y, int w, int h, SDL_Color color, const char *text, int move_x, int move_y) {
    Button temp_button;
    temp_button.rect.x = x;
    temp_button.rect.y = y;
    temp_button.rect.w = w;
    temp_button.rect.h = h;
    temp_button.color = color;
    temp_button.isClicked = False;
    temp_button.x_move = move_x;
    temp_button.y_move = move_y;
    temp_button.text = text;
    temp_button.color = color;
    return temp_button;
}

// 用于检测一个按钮是否被点击
Bool isButtonClicked(Button *button, int mouseX, int mouseY) {
    if (mouseX > button->rect.x && mouseX < button->rect.x + button->rect.w && mouseY > button->rect.y && mouseY < button->rect.y + button->rect.h) {
        return True;
    } else {
        return False;
    }
}

// 用于生成文字，x y为左上角的坐标
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (textSurface == NULL) {
        // printf("Failed to create surface\n");
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // printf("texture ready\n");
    if (textTexture == NULL) {
        printf("Failed to create texture\n");
        return;
    }

    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
    // printf(" 1\n");
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// 加载一个图像
SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Failed to load surface!check your  path!\n");
        return NULL;
    }
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL) {
        printf("Failed to create new texture!\n");
        return NULL;
    }
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// 通过鼠标位置得到列位置
int get_col_from_mouse(Conway *c, int mouseX) {
    int temp_cell_size;
    int max_row_length = 800;
    int max_col_length = 800;
    if (c->rows <= 0 || c->cols <= 0) {
        return -1;
    }
    if (c->rows == c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else if (c->rows > c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else {
        temp_cell_size = max_col_length / c->cols;
    }

    int gap = temp_cell_size / 6;
    if (gap <= 0) {
        gap = 1;
    } else if (gap > 5) {
        gap = 5;
    }

    int change_col = (mouseX - 30) / (temp_cell_size + gap);
    return change_col;
    ;
}

// 通过鼠标位置得到行位置
int get_row_from_mouse(Conway *c, int mouseY) {
    int temp_cell_size;
    int max_row_length = 800;
    int max_col_length = 800;
    if (c->rows <= 0 || c->cols <= 0) {
        return -1;
    }
    if (c->rows == c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else if (c->rows > c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else {
        temp_cell_size = max_col_length / c->cols;
    }

    int gap = temp_cell_size / 6;
    if (gap <= 0) {
        gap = 1;
    } else if (gap > 5) {
        gap = 5;
    }
    int change_row = (mouseY - 50) / (temp_cell_size + gap);
    return change_row;
}

// 绘制普通的格点以及它的边框
void draw_normal_sdlGrids(SDL_Renderer *renderer, Conway *c) {
    int temp_cell_size;
    int max_row_length = 800;
    int max_col_length = 800;
    if (c->rows <= 0 || c->cols <= 0) {
        return;
    }
    if (c->rows == c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else if (c->rows > c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else {
        temp_cell_size = max_col_length / c->cols;
    }

    int gap = temp_cell_size / 6;
    if (gap <= 0) {
        gap = 1;
    } else if (gap > 5) {
        gap = 5;
    }
    SDL_Rect boarder_rec, boarder_rec2;
    boarder_rec.x = 30 - gap;
    boarder_rec.y = 50 - gap * 2;
    boarder_rec.w = temp_cell_size * c->cols + gap * c->cols + gap;
    boarder_rec.h = gap;
    boarder_rec2.x = 30 - gap * 2;
    boarder_rec2.y = 50 - gap * 2;
    boarder_rec2.w = gap;
    boarder_rec2.h = temp_cell_size * c->rows + gap * c->rows + gap * 3;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            SDL_Rect rect;
            rect.x = j * temp_cell_size + j * gap + 30;
            rect.y = i * temp_cell_size + i * gap + 50;
            rect.w = temp_cell_size;
            rect.h = temp_cell_size;
            if (get_state(c, i, j) == Dead) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
            } else if (get_state(c, i, j) == Alive) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &boarder_rec);
    SDL_RenderFillRect(renderer, &boarder_rec2);
    boarder_rec.x = 30 - gap;
    boarder_rec.y = 50 + gap * c->rows + temp_cell_size * c->rows;

    boarder_rec2.x = c->cols * temp_cell_size + gap * c->cols + 30;
    boarder_rec2.y = 50 - gap * 2;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &boarder_rec);
    SDL_RenderFillRect(renderer, &boarder_rec2);
}

// 绘制特殊的格点，同样包含边框
void draw_special_sdlGrids(SDL_Renderer *renderer, Conway *c) {
    int temp_cell_size;
    int max_row_length = 800;
    int max_col_length = 800;
    if (c->rows <= 0 || c->cols <= 0) {
        return;
    }
    if (c->rows == c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else if (c->rows > c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else {
        temp_cell_size = max_col_length / c->cols;
    }

    int gap = temp_cell_size / 6;
    if (gap <= 0) {
        gap = 1;
    } else if (gap > 5) {
        gap = 5;
    }

    SDL_Rect boarder_rec, boarder_rec2;
    boarder_rec.x = 30 - gap;
    boarder_rec.y = 50 - gap * 2;
    boarder_rec.w = temp_cell_size * c->cols + gap * c->cols + gap;
    boarder_rec.h = gap;
    boarder_rec2.x = 30 - gap * 2;
    boarder_rec2.y = 50 - gap * 2;
    boarder_rec2.w = gap;
    boarder_rec2.h = temp_cell_size * c->rows + gap * c->rows + gap * 3;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            SDL_Rect rect;
            rect.x = j * temp_cell_size + j * gap + 30;
            rect.y = i * temp_cell_size + i * gap + 50;
            rect.w = temp_cell_size;
            rect.h = temp_cell_size;

            if (get_state_special(c, i, j).type == Dead) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
            } else if (get_state_special(c, i, j).type == living_producer) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 黑色
            } else if (get_state_special(c, i, j).type == living_consumer) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &boarder_rec);
    SDL_RenderFillRect(renderer, &boarder_rec2);
    boarder_rec.x = 30 - gap;
    boarder_rec.y = 50 + gap * c->rows + temp_cell_size * c->rows;

    boarder_rec2.x = c->cols * temp_cell_size + gap * c->cols + 30;
    boarder_rec2.y = 50 - gap * 2;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &boarder_rec);
    SDL_RenderFillRect(renderer, &boarder_rec2);
}

// 注意，这个函数仅仅用于判断位置，需要前置检测鼠标点击
Bool isGridClicked(Conway *c, int mouseX, int mouseY) {
    int temp_cell_size;
    int max_row_length = 800;
    int max_col_length = 800;
    if (c->rows <= 0 || c->cols <= 0) {
        return False;
    }
    if (c->rows == c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else if (c->rows > c->cols) {
        temp_cell_size = max_row_length / c->rows;
    } else {
        temp_cell_size = max_col_length / c->cols;
    }

    int gap = temp_cell_size / 6;
    if (gap <= 0) {
        gap = 1;
    } else if (gap > 5) {
        gap = 5;
    }
    if (mouseX >= 30 && mouseY >= 50 && mouseX <= c->cols * temp_cell_size + c->cols * gap + 30 &&
        mouseY <= c->rows * temp_cell_size + c->rows * gap + 50) {
        return True;
    } else {
        return False;
    }
}

// 改变格点状态（普通模式）
void changeGridStateOnClick(Conway *c, int mouseX, int mouseY) {
    int change_row = get_row_from_mouse(c, mouseY);
    int change_col = get_col_from_mouse(c, mouseX);
    if (get_state(c, change_row, change_col) == Dead) {
        set_state(c, change_row, change_col, Alive);
        return;

    } else if (get_state(c, change_row, change_col) == Alive) {
        set_state(c, change_row, change_col, Dead);
        return;
    }
}

// 改变格点状态（特殊模式 死-生产着-消费者转化）
void changeGridStateONCLick_special(Conway *c, int mouseX, int mouseY) {
    int change_row = get_row_from_mouse(c, mouseY);
    int change_col = get_col_from_mouse(c, mouseX);

    cell temp_cell = new_cell_setting();
    if (get_state_special(c, change_row, change_col).type == Dead) {
        cell temp_cell = new_cell_setting();
        temp_cell.type = living_producer;
        set_state_special(c, change_row, change_col, temp_cell);
        return;

    } else if (get_state_special(c, change_row, change_col).type == living_producer) {
        temp_cell.type = living_consumer;
        set_state_special(c, change_row, change_col, temp_cell);
        return;
    } else if (get_state_special(c, change_row, change_col).type == living_consumer) {
        temp_cell.type = Dead;
        set_state_special(c, change_row, change_col, temp_cell);
        return;
    }
}

// 更新错误提示等
void updateCommandReport(char report[4][100], const char *reportText) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(report[i], "") == 0) {
            strcpy(report[i], reportText);
            return;
        }
    }
    strcpy(report[1], report[2]);
    strcpy(report[2], report[3]);
    strcpy(report[3], report[4]);
    strcpy(report[4], reportText);
}

// 绘制悬浮窗口显示一些信息
void showCellInformationSpecial(SDL_resource *sdl) {
    int change_row = get_row_from_mouse(sdl->c, sdl->mouseY);
    int change_col = get_col_from_mouse(sdl->c, sdl->mouseX);

    TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
    Button float_information = createButton(sdl->mouseX, sdl->mouseY, 230, 100, (SDL_Color){50, 150, 225, 150}, "Grid information:", 8, 8);
    SDL_Color ButtontextColor = {0, 0, 0};
    SDL_SetRenderDrawColor(sdl->renderer, float_information.color.r, float_information.color.g, float_information.color.b, float_information.color.a);
    SDL_RenderFillRect(sdl->renderer, &float_information.rect);
    renderText(sdl->renderer, row_col_font, "Grid information:", ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 8);

    // 具体的信息
    char row_col_type_information[50];
    char age_hunger_repro[50];
    char tem[50];
    char oxy_or_light[50];
    const char *typeChar[10] = {"Dead", "Alive", "Producer", "Consumer", "Barrier"};
    const char *gene_tem[10] = {"Cold_resistant", "Cold_prefer", "Normal", "Warm_prefer", "Heat resistant"};
    const char *gene_light[10] = {"Dark_resistant", "Low_light prefer", "Normal", "High light prefer", "Extreme light resistant"};
    const char *gene_oxy[10] = {"Low oxygen resistant", "Low oxygen prefer", "Normal", "High oxygen prefer", "Extreme oxygen resistant"};
    sprintf(row_col_type_information, "Row:%d Col:%d Food:%d", change_row, change_col, get_food_state(sdl->c, change_row, change_col));

    sprintf(age_hunger_repro, "Age:%d Hunger:%d Type:%s", get_state_special(sdl->c, change_row, change_col).age,
            get_state_special(sdl->c, change_row, change_col).hunger, typeChar[get_state_special(sdl->c, change_row, change_col).type]);

    // 对于活细胞显示基因信息
    if (get_state_special(sdl->c, change_row, change_col).type != Dead && get_state_special(sdl->c, change_row, change_col).type != barrier) {
        sprintf(tem, "Temperature Preference:%s", gene_tem[get_state_special(sdl->c, change_row, change_col).gene_setting.temper_preference]);

    } else {
        sprintf(tem, "No gene for dead cell!");
    }
    // 生产者和消费者显示不一样
    if (get_state_special(sdl->c, change_row, change_col).type == living_producer) {
        sprintf(oxy_or_light, "Light Preference:%s", gene_oxy[get_state_special(sdl->c, change_row, change_col).gene_setting.light_preference]);

    } else if (get_state_special(sdl->c, change_row, change_col).type == living_consumer) {
        sprintf(oxy_or_light, "Oxygen Preference:%s", gene_light[get_state_special(sdl->c, change_row, change_col).gene_setting.oxy_preference]);

    } else {
        sprintf(oxy_or_light, "No gene for dead cell!");
    }

    renderText(sdl->renderer, row_col_font, row_col_type_information, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 26);
    renderText(sdl->renderer, row_col_font, age_hunger_repro, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 44);
    renderText(sdl->renderer, row_col_font, tem, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 62);
    renderText(sdl->renderer, row_col_font, oxy_or_light, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 80);

    SDL_Rect clipRect = {sdl->mouseX, sdl->mouseY, 200, 100};
    SDL_RenderSetClipRect(sdl->renderer, &clipRect);
    // ... 执行局部渲染操作 ...
    SDL_RenderPresent(sdl->renderer);
    SDL_RenderSetClipRect(sdl->renderer, NULL);
    TTF_CloseFont(row_col_font);
}

// 显示事件列表
void showEventList(Conway *c, SDL_Renderer *renderer, Button EventList) {
    TTF_Font *eventFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
    char event_1[100], event_2[100], event_3[100], event_4[100], event_5[100];
    char event_6[100], event_7[100], event_8[100], event_9[100], event_10[100];
    char *eventChar[20] = {event_1, event_2, event_3, event_4, event_5, event_6, event_7, event_8, event_9, event_10};
    const char *eventList[20] = {"ice_age", "volcano_eruption", "drought", "warm_period", "blue_algal_bloom", "heat_wave"};

    for (int i = 0; i < 10; i++) {
        if (c->currentEvent[i] != none_event && c->event_remain_turn[i] != 0) {
            sprintf(eventChar[i], "Current Event %d: %s Remain Turn:%d", i + 1, eventList[(int)c->currentEvent[i] - 1], c->event_remain_turn[i]);
            renderText(renderer, eventFont, eventChar[i], (SDL_Color){35, 0, 255}, 950, 765 + i * 15);
        }
    }
    renderText(renderer, eventFont, "Event List", (SDL_Color){35, 0, 255}, EventList.rect.x + 8, EventList.rect.y + 8);
}

// 绘制游戏界面的按钮
void showButtons(SDL_resource *sdl, int maxButton) {
    for (int i = 0; i < maxButton; i++) {
        if (sdl->ButtonList[i].rect.w != 0 && sdl->ButtonList[i].rect.h != 0) {
            SDL_SetRenderDrawColor(sdl->renderer, sdl->ButtonList[i].color.r, sdl->ButtonList[i].color.g, sdl->ButtonList[i].color.b,
                                   sdl->ButtonList[i].color.a);
            SDL_RenderFillRect(sdl->renderer, &sdl->ButtonList[i].rect);

            renderText(sdl->renderer, sdl->FontList[2], sdl->ButtonList[i].text, sdl->colorList[0],
                       sdl->ButtonList[i].rect.x + sdl->ButtonList[i].x_move, sdl->ButtonList[i].rect.y + sdl->ButtonList[i].y_move);
        }
    }
}

// 绘制菜单界面
void showMenu(SDL_resource *sdl) {
    Button startButton = createButton(600, 450, 300, 100, (SDL_Color){50, 150, 210, 255}, "", 36, 28);
    Button StartSpecialButton = createButton(600, 600, 300, 100, (SDL_Color){50, 150, 210, 255}, "", 36, 28);
    Button quitButton = createButton(600, 750, 300, 100, (SDL_Color){50, 150, 210, 255}, "", 90, 28);
    // 下面三项用于清除UI中的所有内容

    // 画游戏UI

    // 绘制背景图
    SDL_RenderClear(sdl->renderer);
    SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
    // 绘制主菜单三个按钮
    SDL_SetRenderDrawColor(sdl->renderer, startButton.color.r, startButton.color.g, startButton.color.b, startButton.color.a);
    SDL_RenderFillRect(sdl->renderer, &startButton.rect);
    SDL_RenderFillRect(sdl->renderer, &StartSpecialButton.rect);
    SDL_RenderFillRect(sdl->renderer, &quitButton.rect);
    // 文字渲染
    SDL_Color ButtontextColor = {35, 0, 255};
    renderText(sdl->renderer, sdl->FontList[0], "Game Of Life", ButtontextColor, 460, 180);
    renderText(sdl->renderer, sdl->FontList[1], "Start Normal Mode", ButtontextColor, startButton.rect.x + 36, startButton.rect.y + 28);
    renderText(sdl->renderer, sdl->FontList[1], "Start Special Mode", ButtontextColor, StartSpecialButton.rect.x + 36,
               StartSpecialButton.rect.y + 28);
    renderText(sdl->renderer, sdl->FontList[1], "Quit Game", ButtontextColor, quitButton.rect.x + 90, quitButton.rect.y + 28);
    SDL_RenderPresent(sdl->renderer);
    SDL_Delay(50);
    while (SDL_PollEvent(&sdl->e) != 0) { // 事件处理
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_mainGame = False;
            break;
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
            if (isButtonClicked(&startButton, sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_startPlay = True;
                sdl->flagList.flag_menuButton = False;
            } else if (isButtonClicked(&StartSpecialButton, sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_startSpecial = True;
                sdl->flagList.flag_menuButton = False;
                // printf("yes");
            } else if (isButtonClicked(&quitButton, sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_mainGame = False;
                break;
            }
        }
    }
}

// 随机生成格点
void mouseDown_init_normal(SDL_resource *sdl) {
    // printf("%d\n", sdl->c->probability);
    // printf("Rows:%d\nCols:%d\n ", sdl->c->rows, sdl->c->cols);
    if (sdl->c->probability <= 0 || sdl->c->probability > 100) {
        sdl->c->probability = 40;
    }
    int temp_probability = sdl->c->probability;
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        printf("%d", sizeof(sdl->c->normal_grids));
        delete_grids(sdl->c);
        safeFree(sdl->c);
        if (sdl->temp_row <= 0 || sdl->temp_col <= 0) {
            sdl->temp_row = 20;
            sdl->temp_col = 20;
        }
        sdl->c = update_conway(sdl->temp_row, sdl->temp_col);
        sdl->c->probability = temp_probability;
        printf("%d\n", temp_probability);
        init_random(sdl->c);
        print_conway(sdl->c);
        draw_normal_sdlGrids(sdl->renderer, sdl->c);
        // SDL_RenderPresent(sdl->renderer);
        sdl->flagList.flag_Receive_event = False;
    } else if (sdl->c->rows == 0 || sdl->c->cols == 0) {
        free(sdl->c);
        sdl->c = update_conway(20, 20);
        updateCommandReport(sdl->commandReport, "Warning:Invalid Rows/Cols,using default setting!");
        init_random(sdl->c);
    }
    sdl->flagList.flag_Receive_event = False; // 重新绘制
}

// 更新下一世代
void mouseDown_nextGen_normal(SDL_resource *sdl) {
    if (sdl->c->rows <= 0 || sdl->c->cols <= 0) {
        updateCommandReport(sdl->commandReport, "Warning:Grids haven't been initialized!");
        sdl->flagList.flag_Receive_event = False;
    }
    next_generation(sdl->c);
    sdl->flagList.flag_Receive_event = False; // 重新绘制
}

// 开启输入行列概率1的模式
void mouseDown_basicFeature_normal(SDL_resource *sdl) {
    sdl->flagList.input_mode = True;
    sdl->flagList.input_rows = isButtonClicked(&sdl->ButtonList[2], sdl->mouseX, sdl->mouseY);
    sdl->flagList.input_cols = isButtonClicked(&sdl->ButtonList[3], sdl->mouseX, sdl->mouseY);
    sdl->flagList.input_probability = isButtonClicked(&sdl->ButtonList[4], sdl->mouseX, sdl->mouseY);
}

// 回到菜单
void mouseDown_backMenu_normal(SDL_resource *sdl) {
    sdl->flagList.flag_startPlay = False;
    sdl->flagList.flag_autoMode = False;
    sdl->flagList.flag_menuButton = True;
    sdl->c->last_rows = sdl->c->rows;
    delete_grids(sdl->c);
    printf("%d %d\n", sdl->c->rows, sdl->c->cols);
    // sdl->c=new_conway_special(0,0);
    sdl->flagList.flag_Receive_event = False;
}

// 所有细胞死亡
void mouseDown_reset_normal(SDL_resource *sdl) {
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        for (int i = 0; i < sdl->c->rows; i++) {
            for (int j = 0; j < sdl->c->cols; j++) {
                set_state(sdl->c, i, j, Dead);
            }
        }
        sdl->flagList.flag_Receive_event = False;
    }
}

// 处理所有鼠标按下的情况
void mouseDown_normal(SDL_resource *sdl) {
    SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
    sdl->inputPos = 0;
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
    sdl->flagList.input_mode = False;
    if (isButtonClicked(&sdl->ButtonList[nextGenerationButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_nextGen_normal(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[init_randomButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_init_normal(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY) ||
               isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY) ||
               isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_basicFeature_normal(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_autoMode = True;
        sdl->flagList.flag_Receive_event = False;
    } else if (isButtonClicked(&sdl->ButtonList[importPathButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_importPath = True;
        sdl->flagList.flag_savePath = False;
        sdl->flagList.flag_setTime = False;
    } else if (isButtonClicked(&sdl->ButtonList[savePathButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_savePath = True;
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_setTime = False;
    } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_backMenu_normal(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[timeSetButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_setTime = True;
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_savePath = False;
    } else if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True) {
        changeGridStateOnClick(sdl->c, sdl->mouseX, sdl->mouseY);
        sdl->flagList.flag_Receive_event = False;
    } else if (isButtonClicked(&sdl->ButtonList[resetGridButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_reset_normal(sdl);
    }
}

// 使用renderText函数绘制inputBuffer的内容
void render_input_feature(SDL_resource *sdl) {
    SDL_Color textColor = {2, 200, 0};

    if (sdl->flagList.input_rows == True) {
        renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomInputBox_rowsNum].rect.x + 10,
                   sdl->ButtonList[init_randomInputBox_rowsNum].rect.y + 15);
    } else if (sdl->flagList.input_cols == True) {
        renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomInputBox_colsNum].rect.x + 10,
                   sdl->ButtonList[init_randomInputBox_colsNum].rect.y + 15);
    } else if (sdl->flagList.input_probability == True) {
        renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomButton_proNum].rect.x + 10,
                   sdl->ButtonList[init_randomButton_proNum].rect.y + 15);
    } else {
        renderText(sdl->renderer, sdl->FontList[2], "Enter your path here:", textColor, sdl->ButtonList[init_randomButton_proNum].rect.x + 10,
                   sdl->ButtonList[init_randomButton_proNum].rect.y + 15);
    }

    SDL_RenderPresent(sdl->renderer); // 更新显示的内容
}

// 处理用户键盘输入，写进缓冲区
void Path_input_text_nor_spe(SDL_resource *sdl) {
    sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
    sdl->inputPos++;
    if (sdl->inputPos >= 99) {
        updateCommandReport(sdl->commandReport, "Too Long!");
        return;
    }
    sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符,按alt输入字母
}

// 将缓冲区的信息写入rows cols等
void copy_basicFeature_from_inputBuffer(SDL_resource *sdl) {
    if (sdl->flagList.input_rows == True) {
        sscanf(sdl->inputBuffer, "%d", &sdl->temp_row);
    } else if (sdl->flagList.input_cols == True) {
        sscanf(sdl->inputBuffer, "%d", &sdl->temp_col);
    } else if (sdl->flagList.input_probability == True) {
        sscanf(sdl->inputBuffer, "%d", &sdl->c->probability);
        printf("%d", sdl->c->probability);
    }
}

// 决定用户输入的是行还是列还是概率
void input_basic_feature_modeDecide(SDL_resource *sdl) {
    if (sdl->flagList.flag_startSpecial) {
        if (sdl->c->rows > 0 && sdl->c->cols > 0) {
            int temp_probability = sdl->c->probability;
            sdl->c = new_conway_special(sdl->c->rows, sdl->c->cols);
            sdl->c->probability = temp_probability;
            init_random_special(sdl->c);
        }
    } else if (sdl->flagList.flag_startPlay) {
        if (sdl->c->rows > 0 && sdl->c->cols > 0) {
            int temp_probability = sdl->c->probability;
            sdl->c = update_conway(sdl->c->rows, sdl->c->cols);
            sdl->c->probability = temp_probability;
            init_random(sdl->c);
        }
    }
}

// 输入基本信息的函数
void input_basic_feature(SDL_resource *sdl) {
    if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
        Path_input_text_nor_spe(sdl);
        printf("用户输入了: %s\n", sdl->inputBuffer);
        copy_basicFeature_from_inputBuffer(sdl);
        input_basic_feature_modeDecide(sdl);
    }
    render_input_feature(sdl);
}

// 输入路径并按下回车后的情况处理
void Path_input_return_normal(SDL_resource *sdl) {
    if (sdl->flagList.flag_importPath == True) {
        sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
        delete_grids(sdl->c);
        Conway *temp_c = new_conway_from_file(sdl->c, sdl->inputBuffer);
        if (temp_c != NULL) {
            sdl->c = temp_c;
        } else {
            updateCommandReport(sdl->commandReport, "Warning:Invalid Path!");
        }
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_Receive_event = False;
    } else if (sdl->flagList.flag_savePath == True) {
        sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
        save_conway(sdl->c, sdl->inputBuffer);
        sdl->flagList.flag_savePath = False;
        sdl->flagList.flag_Receive_event = False;
    }
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
}

// 在输入栏显示用户输入的路径
void render_path_input_normal(SDL_resource *sdl) {
    SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

    if (sdl->flagList.flag_importPath == True) {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[importPathButtonNum].rect.x + 10,
                   sdl->ButtonList[importPathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    } else if (sdl->flagList.flag_savePath == True) {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[savePathButtonNum].rect.x + 10,
                   sdl->ButtonList[savePathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    } else {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[importPathButtonNum].rect.x + 10,
                   sdl->ButtonList[importPathButtonNum].rect.y + 15);
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[savePathButtonNum].rect.x + 10,
                   sdl->ButtonList[savePathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    }
}

// 同样，将输入的值写入缓冲区
void Path_input_normal(SDL_resource *sdl) {

    if (sdl->e.key.keysym.sym != SDLK_LALT) {
        Path_input_text_nor_spe(sdl);
    }
    if (sdl->e.key.keysym.sym == SDLK_0) {
        sdl->inputBuffer[0] = '\0';
    } else if (sdl->e.key.keysym.sym == SDLK_RETURN) {
        Path_input_return_normal(sdl);
    }
    render_path_input_normal(sdl);
}

// 输入时间
void set_time(SDL_resource *sdl) {
    if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
        // 检查输入的是数字
        sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
        sdl->inputPos++;
        if (sdl->inputPos >= 99) {
            sdl->autoTime = 1000;
            updateCommandReport(sdl->commandReport, "Warning:Invalid Time input!");
            sdl->flagList.flag_setTime = False;
            return;
        }
        sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符
        printf("用户输入了: %s\n", sdl->inputBuffer);
        sscanf(sdl->inputBuffer, "%d", &sdl->autoTime);
        if (sdl->autoTime < 80) {
            sdl->autoTime = 80;
        }
    }
    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
    renderText(sdl->renderer, pathFont, sdl->inputBuffer, sdl->colorList[0], sdl->ButtonList[timeSetButtonNum].rect.x + 10,
               sdl->ButtonList[timeSetButtonNum].rect.y + 15);
}

// 处理用户交互功能(普通模式)
void Normal_mode_choice(SDL_resource *sdl) {
    sdl->c->last_rows = sdl->c->rows;
    SDL_StopTextInput();
    sdl->flagList.flag_Receive_event = True;
    while (SDL_WaitEvent(&sdl->e) != 0 && sdl->flagList.flag_Receive_event) { // 事件处理
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            mouseDown_normal(sdl);
            // 文字输入的处理
        } else if (sdl->flagList.input_mode == True && sdl->e.type == SDL_KEYDOWN) {
            input_basic_feature(sdl);
            // 处理文件名的输入
        } else if ((sdl->flagList.flag_importPath == True || sdl->flagList.flag_savePath == True) && sdl->e.type == SDL_KEYDOWN) {
            Path_input_normal(sdl);
            // 设置时间
        } else if (sdl->flagList.flag_setTime == True && sdl->e.type == SDL_KEYDOWN) {
            set_time(sdl);
        }
        SDL_RenderPresent(sdl->renderer);
    }
    sdl->flagList.flag_setTime = False;
}
// 特殊模式更新世代
void mouseDown_nextGen_special(SDL_resource *sdl) {
    next_generation_special(sdl->c);
    sdl->c->grid_feature.count_turn += 1;
    if (sdl->c->rows <= 0 || sdl->c->cols <= 0) {
        updateCommandReport(sdl->commandReport, "Warning:Grids haven't been initialized!");
        sdl->flagList.flag_Receive_event = False;
    }
    sdl->flagList.flag_Receive_event = False; // 重新绘制
}

// 特殊模式随机初始化格点
void mouseDown_init_special(SDL_resource *sdl) {
    printf("%d\n", sdl->c->probability);
    int temp_probability = sdl->c->probability;
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        delete_special_grids(sdl->c);
        safeFree(sdl->c);
        sdl->c = new_conway_special(sdl->temp_row, sdl->temp_col);
        sdl->c->probability = temp_probability;
        printf("%d\n", temp_probability);
        init_random_special(sdl->c);
    } else if (sdl->c->rows == 0 || sdl->c->cols == 0) {
        safeFree(sdl->c);
        sdl->c = new_conway_special(20, 20);
        updateCommandReport(sdl->commandReport, "Warning:Invalid Rows/Cols,using default setting!");
        init_random_special(sdl->c);
    }
    sdl->flagList.flag_Receive_event = False; // 重新绘制
}

// 特殊模式开始输入基本信息
void mouseDown_basicFeature_special(SDL_resource *sdl) {
    sdl->flagList.input_mode = True;
    sdl->flagList.input_rows = isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY); // 防止两个一起变成真
    sdl->flagList.input_cols = isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY);
    sdl->flagList.input_probability = isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY);
}

// 特殊模式回到菜单
void mouseDown_backMenu_special(SDL_resource *sdl) {
    sdl->flagList.flag_startSpecial = False;
    sdl->flagList.flag_autoMode_special = False;
    sdl->flagList.flag_menuButton = True;
    delete_special_grids(sdl->c);
    // sdl->c=update_conway(0,0);
    sdl->flagList.flag_Receive_event = False;
}

// 特殊模式开启编辑格点的状态
void mouseDown_editMode_special(SDL_resource *sdl) {
    changeGridStateONCLick_special(sdl->c, sdl->mouseX, sdl->mouseY);
    draw_special_sdlGrids(sdl->renderer, sdl->c);
    SDL_RenderPresent(sdl->renderer);
}

// 特殊模式将所有细胞设置为死亡
void mouseDown_reset_special(SDL_resource *sdl) {
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        for (int i = 0; i < sdl->c->rows; i++) {
            for (int j = 0; j < sdl->c->cols; j++) {
                set_state_special(sdl->c, i, j, new_cell_setting());
            }
        }
        sdl->c->grid_feature.count_turn = 0;
        sdl->flagList.flag_Receive_event = False;
    }
}

// 特殊模式开启/关闭编辑格点模式
void mouseDown_change_editMode_special(SDL_resource *sdl) {
    if (sdl->flagList.flag_editGrid == False) {
        sdl->flagList.flag_editGrid = True;
        renderText(sdl->renderer, sdl->FontList[2], "Disable edit on click", (SDL_Color){255, 0, 0}, sdl->ButtonList[editModeButtonNum].rect.x + 18,
                   sdl->ButtonList[editModeButtonNum].rect.y + 8);
        updateCommandReport(sdl->commandReport, "Yor are editing grids on click!");
    } else {
        sdl->flagList.flag_editGrid = False;
    }
    sdl->flagList.flag_Receive_event = False;
}

// 处理特殊模式的所有鼠标交互
void mouseDown_special(SDL_resource *sdl) {
    SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
    sdl->inputPos = 0;
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
    sdl->flagList.input_mode = False;
    if (isButtonClicked(&sdl->ButtonList[nextGenerationButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_nextGen_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[init_randomButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_init_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY) ||
               isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY) ||
               isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_basicFeature_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_autoMode_special = True;
        sdl->flagList.flag_Receive_event = False;
    } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_backMenu_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[timeSetButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_setTime = True;
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_savePath = False;
    } else if (isButtonClicked(&sdl->ButtonList[importPathButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_importPath = True;
        sdl->flagList.flag_savePath = False;
        sdl->flagList.flag_setTime = False;
    } else if (isButtonClicked(&sdl->ButtonList[savePathButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_savePath = True;
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_setTime = False;
    } else if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True && sdl->flagList.flag_editGrid == True) {
        mouseDown_backMenu_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[resetGridButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_reset_special(sdl);
    } else if (isButtonClicked(&sdl->ButtonList[editModeButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_change_editMode_special(sdl);
    }
}

// 处理特殊模式下路径输入的回车
void Path_input_return_special(SDL_resource *sdl) {
    if (sdl->flagList.flag_importPath == True) {
        sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
        delete_special_grids(sdl->c);
        Conway *temp_c = new_conway_from_file_special(sdl->c, sdl->inputBuffer);
        if (temp_c != NULL) {
            sdl->c = temp_c;
        } else {
            updateCommandReport(sdl->commandReport, "Warning:Invalid Path!");
        }
        sdl->flagList.flag_importPath = False;
        sdl->flagList.flag_Receive_event = False;
    } else if (sdl->flagList.flag_savePath == True) {
        sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
        save_conway_special(sdl->c, sdl->inputBuffer);
        sdl->flagList.flag_savePath = False;
        sdl->flagList.flag_Receive_event = False;
    }
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
}

// 绘制缓冲区
void render_inputBuffer_special(SDL_resource *sdl) {
    SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);
    if (sdl->flagList.flag_importPath == True) {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[importPathButtonNum].rect.x + 10,
                   sdl->ButtonList[importPathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    } else if (sdl->flagList.flag_savePath == True) {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[savePathButtonNum].rect.x + 10,
                   sdl->ButtonList[savePathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    } else {
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[importPathButtonNum].rect.x + 10,
                   sdl->ButtonList[importPathButtonNum].rect.y + 15);
        renderText(sdl->renderer, pathFont, sdl->inputBuffer, textColor, sdl->ButtonList[savePathButtonNum].rect.x + 10,
                   sdl->ButtonList[savePathButtonNum].rect.y + 15);
        SDL_RenderPresent(sdl->renderer);
    }
}

// 特殊模式输入路径
void Path_input_special(SDL_resource *sdl) {
    if (sdl->e.key.keysym.sym != SDLK_LALT) {
        Path_input_text_nor_spe(sdl);
    }
    if (sdl->e.key.keysym.sym == SDLK_0) {
        sdl->inputBuffer[0] = '\0';
    } else if (sdl->e.key.keysym.sym == SDLK_RETURN) {
        Path_input_return_special(sdl);
    }
    render_inputBuffer_special(sdl);
}

// 特殊模式展示悬浮窗口
void show_float_window(SDL_resource *sdl, int *lastHoveredRow, int *lastHoveredCol) {
    static Bool isWindowShown = False;
    int currentRow = get_row_from_mouse(sdl->c, sdl->mouseX);
    int currentCol = get_col_from_mouse(sdl->c, sdl->mouseY);

    if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True) {
        if ((currentRow != *lastHoveredRow || currentCol != *lastHoveredCol)) {
            SDL_Delay(100);
            showCellInformationSpecial(sdl);
            isWindowShown = True; // 设置窗口为显示状态
            *lastHoveredRow = currentRow;
            *lastHoveredCol = currentCol;
            while (SDL_WaitEvent(&sdl->e) == 0) {}
            SDL_Delay(30);
            sdl->flagList.flag_Receive_event = False;
        }
    } else {
        if (isWindowShown) {
            // 这里关闭或隐藏信息窗口
            isWindowShown = False;
        }
    }
}

// 特殊模式的所有交互内容
void Special_mode_choice(SDL_resource *sdl) {
    SDL_StopTextInput();
    sdl->flagList.flag_Receive_event = True;
    int lastHoveredRow = -1;
    int lastHoveredCol = -1;
    while (SDL_WaitEvent(&sdl->e) != 0 && sdl->flagList.flag_Receive_event) {
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startSpecial = False;
            sdl->flagList.flag_mainGame = False;
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            mouseDown_special(sdl);
        } else if (sdl->flagList.input_mode == True && sdl->e.type == SDL_KEYDOWN) {
            input_basic_feature(sdl);
        } else if ((sdl->flagList.flag_importPath == True || sdl->flagList.flag_savePath == True) && sdl->e.type == SDL_KEYDOWN) {
            Path_input_special(sdl);
            // 设置时间
        } else if (sdl->flagList.flag_setTime == True && sdl->e.type == SDL_KEYDOWN) {
            set_time(sdl);

        } else if (sdl->e.type != SDL_MOUSEBUTTONDOWN && sdl->flagList.flag_editGrid == False) {
            show_float_window(sdl, &lastHoveredRow, &lastHoveredCol);
        }
        SDL_RenderPresent(sdl->renderer);
    }
    sdl->flagList.flag_setTime = False;
}

// 绘制显示基本信息的样板（普通模式）
void draw_normal_informationBoard(SDL_resource *sdl) {
    SDL_Color ButtontextColor = {35, 0, 255};
    sprintf(sdl->BasicGridInformation[0], "Rows:%d  Cols:%d  livingCells:%d  Auto Mode Pause time:%dms", sdl->c->rows, sdl->c->cols,
            count_living_cell_normal(sdl->c), sdl->autoTime);
    renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation[0], ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 45);
    for (int i = 0; i < 4; i++) {
        renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport[i], ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
                   sdl->ButtonList[informationBoardNum].rect.y + 60 + i * 15);
        memset(sdl->commandReport[i], '\0', sizeof(sdl->commandReport[i]));
    }

    // 初始化命令
}

// 信息和事件列表
void draw_special_informationBoard(SDL_resource *sdl) {
    SDL_Color ButtontextColor = {35, 0, 255};

    const char *temLevel[] = {"freezing", " cold", "cold", "cool", "cool", "normal", "warm", "warm", "hot", "hot", "extreme_hot"};
    const char *oxyLevel[] = {"extreme low", "low", "miner low", "normal", "abundant", "high", "extreme high"};
    const char *lightLevel[] = {"dark", "low", "low", "normal", "abundant", "high", "extreme high"};

    sprintf(sdl->BasicGridInformation[0], "Rows:%d  Cols:%d   Auto Mode Pause time:%dms", sdl->c->rows, sdl->c->cols, sdl->autoTime);
    sprintf(sdl->BasicGridInformation[1], "Producer amount:%d  Consumer amount:%d  Current turn:%d", count_living_producer(sdl->c),
            count_living_consumer(sdl->c), sdl->c->grid_feature.count_turn);
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        sprintf(sdl->BasicGridInformation[2], "Temperature:%s  Oxygen:%s  Light:%s", temLevel[sdl->c->grid_feature.temperature],
                oxyLevel[sdl->c->grid_feature.oxygen_resource], lightLevel[sdl->c->grid_feature.light_resource]);
    }

    for (int i = 0; i < 3; i++) {
        renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation[i], ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
                   sdl->ButtonList[informationBoardNum].rect.y + 45 + i * 15);
    }

    for (int i = 0; i < 4; i++) {
        renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport[i], ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
                   sdl->ButtonList[informationBoardNum].rect.y + 100 + i * 15);
        memset(sdl->commandReport[i], '\0', sizeof(sdl->commandReport[i]));
    }

    showEventList(sdl->c, sdl->renderer, sdl->ButtonList[EventListNum]);
}

// 普通——自动模式下的鼠标交互
void mouseDownAuto_normal(SDL_resource *sdl) {
    if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_autoMode = False;
        sdl->flagList.flag_Receive_event = False;
    } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_backMenu_normal(sdl);
    }
}

// 普通——自动模式的交互
void NormalAutoModeChoice(SDL_resource *sdl) {
    while (SDL_PollEvent(&sdl->e) != 0) { // 事件处理
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            sdl->flagList.flag_autoMode = False;
            // delete_grids(sdl->c);
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            mouseDownAuto_normal(sdl);
        }
    }
}

// 特殊——自动模式的鼠标交互
void mouseDownAuto_special(SDL_resource *sdl) {
    if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
        sdl->flagList.flag_autoMode_special = False;
        sdl->flagList.flag_Receive_event = False;
    } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
        mouseDown_backMenu_special(sdl);
    }
}

// 特殊——自动模式显示悬浮窗口
void HoverWindow_special(SDL_resource *sdl) {
    int lastHoveredRow = -1;
    int lastHoveredCol = -1;
    static Bool isWindowShown = False;
    int currentRow = get_row_from_mouse(sdl->c, sdl->mouseX);
    int currentCol = get_col_from_mouse(sdl->c, sdl->mouseY);

    if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True) {
        if ((currentRow != lastHoveredRow || currentCol != lastHoveredCol)) {
            SDL_Delay(100);
            showCellInformationSpecial(sdl);
            isWindowShown = True; // 设置窗口为显示状态
            lastHoveredRow = currentRow;
            lastHoveredCol = currentCol;
            while (SDL_WaitEvent(&sdl->e) == 0) {}
            SDL_Delay(300);
            sdl->flagList.flag_Receive_event = False;
        }
    } else {
        if (isWindowShown) {
            // 这里关闭或隐藏信息窗口
            isWindowShown = False;
        }
    }
}

// 特殊——自动模式交互
void SpecialAutoModeChoice(SDL_resource *sdl) {
    sdl->flagList.flag_Receive_event = True;
    while (SDL_PollEvent(&sdl->e) != 0 && sdl->flagList.flag_Receive_event) { // 事件处理
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_autoMode_special = False;
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            // delete_special_grids(sdl->c);
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            mouseDownAuto_special(sdl);
        } else if (sdl->e.type != SDL_MOUSEBUTTONDOWN && sdl->flagList.flag_editGrid == False) {
            HoverWindow_special(sdl);
        }
    }
}

// 覆盖原来的颜色
void changeButtonTextColor(SDL_resource *sdl) {
    SDL_Color ButtontextColor_autoControl = {255, 0, 0};
    if (sdl->flagList.flag_autoMode == True) {
        renderText(sdl->renderer, sdl->FontList[2], "start/stop AutoMode", ButtontextColor_autoControl,
                   sdl->ButtonList[startAutoModeButtonNum].rect.x + 8,
                   sdl->ButtonList[startAutoModeButtonNum].rect.y + 8); // 特殊颜色的按钮
    } else if (sdl->flagList.flag_autoMode_special == True) {
        renderText(sdl->renderer, sdl->FontList[2], "start/stop AutoMode", ButtontextColor_autoControl,
                   sdl->ButtonList[startAutoModeButtonNum].rect.x + 8,
                   sdl->ButtonList[startAutoModeButtonNum].rect.y + 8); // 特殊颜色的按钮
    } else if (sdl->flagList.flag_editGrid == True) {
        SDL_SetRenderDrawColor(sdl->renderer, sdl->ButtonList[editModeButtonNum].color.r, sdl->ButtonList[editModeButtonNum].color.g,
                               sdl->ButtonList[editModeButtonNum].color.b, sdl->ButtonList[editModeButtonNum].color.a);
        SDL_RenderFillRect(sdl->renderer, &sdl->ButtonList[editModeButtonNum].rect);
        renderText(sdl->renderer, sdl->FontList[2], "Disable edit on click", (SDL_Color){255, 0, 0}, sdl->ButtonList[editModeButtonNum].rect.x + 18,
                   sdl->ButtonList[editModeButtonNum].rect.y + 8);
    }
}

// 释放所有SDL资源和sdl以外的动态分配的内存
void free_all_resource(SDL_resource *sdl) {
    if (sdl->LastPlayedMode == 0) {
        sdl->c->last_rows = sdl->c->rows;
        delete_grids(sdl->c);
    } else if (sdl->LastPlayedMode == 1) {
        delete_special_grids(sdl->c);
    }
    safeFree(sdl->c);
    if (sdl == NULL)
        return;

    if (sdl->texture != NULL) {
        SDL_DestroyTexture(sdl->texture);
        sdl->texture = NULL;
    }
    if (sdl->renderer != NULL) {
        SDL_DestroyRenderer(sdl->renderer);
        sdl->renderer = NULL;
    }
    if (sdl->window != NULL) {
        SDL_DestroyWindow(sdl->window);
        sdl->window = NULL;
    }

    for (int i = 0; i < 20; i++) {
        if (sdl->FontList[i] != NULL) {
            TTF_CloseFont(sdl->FontList[i]);
            sdl->FontList[i] = NULL;
        }
    }
}

// 初始化所有SDL资源 包括按钮 Conway结构体 SDL结构体 背景 字体 颜色 等等
SDL_resource *initialize_all() {
    srand(time(NULL));
    SDL_resource *sdl = (SDL_resource *)malloc(sizeof(SDL_resource));
    Conway *c = update_conway(0, 0);
    c->grid_feature.count_turn = 0;
    sdl->c = c;
    sdl->c->probability = 50;
    sdl->autoTime = 1000;
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
    sdl->inputPos = 0;

    if (initialize_sdl() == False) {
        delete_grids(c);
        safeFree(sdl->c);
        safeFree(sdl);
        exit(1);
    }

    TTF_Init(); // 初始化SDL以及ttf（文字处理）

    for (int i = 0; i < 20; i++) {
        sdl->FontList[i] = NULL;
    }
    for (int i = 0; i < 40; i++) {
        sdl->ButtonList[i] = initializeButton();
    }

    SDL_Window *window = init_window(1520, 1080);
    SDL_Renderer *renderer = render_window(window);
    SDL_Texture *background = loadTexture(renderer, "./pic/ocean.jpg");
    SDL_SetWindowTitle(window, "Game of Life");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Button backToMenuButton = createButton(5, 5, 50, 30, (SDL_Color){50, 150, 210, 255}, "Menu", 3, 8);
    Button init_randomButton = createButton(950, 100, 170, 40, (SDL_Color){50, 150, 210, 255}, "Init grids(Default 20*20)", 3, 8);
    Button init_randomInputBox_rows = createButton(1130, 100, 60, 40, (SDL_Color){255, 255, 255, 255}, "Rows:", 0, 0);
    Button init_randomInputBox_cols = createButton(1210, 100, 60, 40, (SDL_Color){255, 255, 255, 255}, "Cols:", 0, 0);
    Button init_randomButton_pro = createButton(1290, 100, 120, 40, (SDL_Color){255, 255, 255, 255}, "Probability:(%)", 0, 0);
    Button startAutoModeButton = createButton(950, 160, 170, 40, (SDL_Color){50, 150, 210, 255}, "start/stop AutoMode", 8, 8);
    Button nextGenerationButton = createButton(950, 220, 170, 40, (SDL_Color){50, 150, 210, 255}, "Next Generation", 26, 8);
    Button importChoiceButton = createButton(950, 280, 100, 40, (SDL_Color){50, 150, 210, 255}, "Load File", 18, 8);
    Button importPathButton = createButton(1080, 280, 240, 40, (SDL_Color){255, 255, 255, 255}, "Click to enter your path here:", 10, 0);
    Button saveChoiceButton = createButton(950, 340, 100, 40, (SDL_Color){50, 150, 210, 255}, "Save Grids", 14, 8);
    Button savePathButton = createButton(1080, 340, 240, 40, (SDL_Color){255, 255, 255, 255}, "Click to enter your path here:", 10, 0);
    Button timeSetButton = createButton(1150, 160, 120, 40, (SDL_Color){50, 150, 210, 255}, "PauseTime(ms):", 0, 0);
    Button resetGridButton = createButton(950, 400, 170, 40, (SDL_Color){50, 150, 210, 255}, "Reset all cells dead", 18, 8);
    Button informationBoard = createButton(950, 460, 450, 300, (SDL_Color){50, 150, 210, 255}, "Grid information:", 8, 8);
    Button editModeButton = createButton(1145, 400, 170, 40, (SDL_Color){50, 150, 210, 255}, "Enable edit on click", 18, 8);
    Button EventList = createButton(950, 740, 450, 300, (SDL_Color){50, 150, 210, 255}, "", 0, 0);

    TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 25);

    TTF_Font *GameName_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 100);
    // 按钮上的文字
    TTF_Font *Button_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 15);

    TTF_Font *informationFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);

    sdl->colorList[0] = (SDL_Color){35, 0, 255};
    sdl->colorList[1] = (SDL_Color){2, 200, 0};

    sdl->window = window;
    sdl->renderer = renderer;
    sdl->texture = background;
    sdl->FontList[0] = GameName_font;
    sdl->FontList[1] = row_col_font;
    sdl->FontList[2] = Button_font;
    sdl->FontList[3] = informationFont;

    sdl->ButtonList[0] = backToMenuButton;
    sdl->ButtonList[1] = init_randomButton;
    sdl->ButtonList[2] = init_randomInputBox_rows;
    sdl->ButtonList[3] = init_randomInputBox_cols;
    sdl->ButtonList[4] = init_randomButton_pro;
    sdl->ButtonList[5] = startAutoModeButton;
    sdl->ButtonList[6] = nextGenerationButton;
    sdl->ButtonList[7] = importChoiceButton;
    sdl->ButtonList[8] = importPathButton;
    sdl->ButtonList[9] = saveChoiceButton;
    sdl->ButtonList[10] = savePathButton;
    sdl->ButtonList[11] = timeSetButton;
    sdl->ButtonList[12] = resetGridButton;
    sdl->ButtonList[13] = informationBoard;
    sdl->ButtonList[14] = editModeButton;
    sdl->ButtonList[15] = EventList;

    sdl->flagList.flag_mainGame = True;
    sdl->flagList.flag_menuButton = True;
    sdl->flagList.flag_autoMode = False;
    sdl->flagList.flag_autoMode_special = False;
    sdl->flagList.flag_importPath = False;
    sdl->flagList.flag_savePath = False;
    sdl->flagList.flag_setTime = False;
    sdl->flagList.flag_startPlay = False;
    sdl->flagList.flag_startSpecial = False;
    sdl->flagList.input_cols = False;
    sdl->flagList.input_rows = False;
    sdl->flagList.input_mode = False;
    sdl->flagList.input_probability = False;
    sdl->flagList.flag_editGrid = False;
    for (int i = 0; i < 4; i++) {
        memset(sdl->commandReport[i], '\0', sizeof(sdl->commandReport[i]));
    }
    for (int i = 0; i < 3; i++) {
        memset(sdl->BasicGridInformation[i], '\0', sizeof(sdl->BasicGridInformation[i]));
    }

    SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
    return sdl;
}

// 防止程序直接异常退出（虽然这样可能导致内存泄漏，但总比越界来的安全）
// 这个函数一般不会起作用，因为前面的函数已经封装好了最大行列数的限制
void safeTest(SDL_resource *sdl) {
    if (sdl->c->rows > 150 || sdl->c->cols > 150) {
        sdl->c->rows = 0;
        sdl->c->cols = 0;
        sdl->c = update_conway(0, 0);
    }
}

// 普通模式的运行逻辑
void normalGame(SDL_resource *sdl) {
    safeTest(sdl);
    sdl->LastPlayedMode = 0;
    SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
    showButtons(sdl, 14); // 前14个按钮，用于普通模式
    changeButtonTextColor(sdl);
    draw_normal_informationBoard(sdl);
    draw_normal_sdlGrids(sdl->renderer, sdl->c);
    SDL_RenderPresent(sdl->renderer);
    Normal_mode_choice(sdl);

    while (sdl->flagList.flag_autoMode == True) {
        SDL_RenderClear(sdl->renderer); // 清除旧的内容
        SDL_Delay(sdl->autoTime);
        next_generation(sdl->c); // 更新Conway游戏的状态
        SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
        showButtons(sdl, 14); // 前14个按钮，用于普通模式
        changeButtonTextColor(sdl);
        draw_normal_informationBoard(sdl);
        draw_normal_sdlGrids(sdl->renderer, sdl->c);
        SDL_RenderPresent(sdl->renderer);
        NormalAutoModeChoice(sdl);
    }
}

// 特殊模式的运行逻辑
void specialGame(SDL_resource *sdl) {
    safeTest(sdl);
    sdl->LastPlayedMode = 1;
    SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
    showButtons(sdl, 16); // 前15个按钮，用于特殊模式
    changeButtonTextColor(sdl);
    draw_special_informationBoard(sdl);
    draw_special_sdlGrids(sdl->renderer, sdl->c);
    SDL_RenderPresent(sdl->renderer);
    Special_mode_choice(sdl);

    while (sdl->flagList.flag_autoMode_special == True) {
        SDL_RenderClear(sdl->renderer); // 清除旧的内容
        SDL_Delay(sdl->autoTime);
        next_generation_special(sdl->c);
        SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
        showButtons(sdl, 16);
        changeButtonTextColor(sdl);
        draw_special_informationBoard(sdl);
        draw_special_sdlGrids(sdl->renderer, sdl->c);
        SDL_RenderPresent(sdl->renderer);
        SpecialAutoModeChoice(sdl);
    }
}

// 主游戏
void mainGame(SDL_resource *sdl) {
    while (sdl->flagList.flag_mainGame == True) {
        if (sdl->flagList.flag_menuButton == True) {
            showMenu(sdl);
        }
        if (sdl->flagList.flag_startPlay == True) {
            normalGame(sdl);
        } else if (sdl->flagList.flag_startSpecial == True) {
            specialGame(sdl);
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_resource *sdl = initialize_all();
    mainGame(sdl);
    free_all_resource(sdl);
    // free(sdl);
    safeFree(sdl);
    SDL_Quit();
    TTF_Quit();
    return 0;
}