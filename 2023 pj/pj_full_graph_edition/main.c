#include "conway.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    Bool isClicked;
    TTF_Font *font;
    const char *text;
    int x_move;
    int y_move;

    // ...其他属性，例如标签、图标等
} Button;

typedef enum {
    backToMenuButtonNum = 0,
    init_randomButtonNum,
    init_randomInputBox_rowsNum,
    init_randomInputBox_colsNum,
    init_randomButton_proNum,
    startAutoModeButtonNum,
    nextGenerationButtonNum,
    importChoiceButtonNum,
    importPathButtonNum,
    saveChoiceButtonNum,
    savePathButtonNum,
    timeSetButtonNum,
    resetGridButtonNum,
    informationBoardNum,
    editModeButtonNum,
    EventListNum,

} button_number;
typedef struct {
    Bool flag_mainGame;
    Bool flag_menuButton;
    Bool flag_startPlay;
    Bool flag_startSpecial;
    Bool flag_autoMode;
    Bool flag_autoMode_special;
    Bool input_mode;
    Bool input_rows;
    Bool input_cols;
    Bool input_probability;
    Bool flag_setTime;
    Bool flag_importPath;
    Bool flag_savePath;
    Bool flag_editGrid;
} flags;

typedef struct {
    Conway *c;
    Button ButtonList[40];
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Color colorList[20];
    TTF_Font *FontList[20];
    flags flagList;
    SDL_Event e;
    int mouseX;
    int mouseY;
    int autoTime;
    int inputPos;
    char inputBuffer[100];
    char BasicGridInformation[100];
    char BasicGridInformation_2[100];
    char BasicGridInformation_3[100];
    char commandReport_1[100];
    char commandReport_2[100];
    char commandReport_3[100];
    char commandReport_4[100];

} SDL_resource;

Button initializeButton() {
    Button newButton;
    newButton.color = (SDL_Color){0, 0, 0};
    newButton.isClicked = False;
    newButton.rect = (SDL_Rect){0};
    return newButton;
}

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
        printf("Failed to create surface\n");
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

void updateCommandReport(char *rep1, char *rep2, char *rep3, char *rep4, const char *reportText) {
    char *report_char[4] = {rep1, rep2, rep3, rep4};
    for (int i = 0; i < 4; i++) {
        if (strcmp(report_char[i], "") == 0) {
            strcpy(report_char[i], reportText);
            return;
        }
    }
    strcpy(rep1, rep2);
    strcpy(rep2, rep3);
    strcpy(rep3, rep4);
    strcpy(rep4, reportText);
}

// 悬浮窗口
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

// 事件列表
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
                printf("yes");
            } else if (isButtonClicked(&quitButton, sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_mainGame = False;
                break;
            }
        }
    }
}

// 处理用户交互功能(普通模式)
void Normal_mode_choice(SDL_resource *sdl) {

    while (SDL_WaitEvent(&sdl->e) != 0) { // 事件处理

        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
            sdl->inputPos = 0;
            memset(sdl->inputBuffer, 0, sizeof(sdl->inputBuffer));
            sdl->flagList.input_mode = False;
            if (isButtonClicked(&sdl->ButtonList[nextGenerationButtonNum], sdl->mouseX, sdl->mouseY)) {
                if (sdl->c->rows <= 0 || sdl->c->cols <= 0) {
                    updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                        "Warning:Grids haven't been initialized!");
                    break;
                }
                next_generation(sdl->c);
                break; // 重新绘制
            } else if (isButtonClicked(&sdl->ButtonList[init_randomButtonNum], sdl->mouseX, sdl->mouseY)) {
                printf("%d\n", sdl->c->probability);
                printf("Rows:%d\nCols:%d\n ", sdl->c->rows, sdl->c->cols);
                if (sdl->c->probability <= 0 || sdl->c->probability > 100) {
                    sdl->c->probability = 40;
                }
                int temp_probability = sdl->c->probability;
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    sdl->c = update_conway(sdl->c->rows, sdl->c->cols);
                    sdl->c->probability = temp_probability;
                    printf("%d\n", temp_probability);

                    init_random(sdl->c);
                    print_conway(sdl->c);
                    draw_normal_sdlGrids(sdl->renderer, sdl->c);
                    SDL_RenderPresent(sdl->renderer);
                    return;
                } else if (sdl->c->rows == 0 || sdl->c->cols == 0) {
                    sdl->c->rows = 20;
                    sdl->c->cols = 20;
                    sdl->c = update_conway(sdl->c->rows, sdl->c->cols);
                    updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                        "Warning:Invalid Rows/Cols,using default setting!");
                    init_random(sdl->c);
                }
                break; // 重新绘制
            } else if (isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY) ||
                       isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY) ||
                       isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.input_mode = True;
                sdl->flagList.input_rows = isButtonClicked(&sdl->ButtonList[2], sdl->mouseX, sdl->mouseY);
                sdl->flagList.input_cols = isButtonClicked(&sdl->ButtonList[3], sdl->mouseX, sdl->mouseY);
                sdl->flagList.input_probability = isButtonClicked(&sdl->ButtonList[4], sdl->mouseX, sdl->mouseY);

            } else if (isButtonClicked(&sdl->ButtonList[5], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode = True;
                break; // auto 的循环在下面，所以可以break;
            } else if (isButtonClicked(&sdl->ButtonList[8], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_importPath = True;
            } else if (isButtonClicked(&sdl->ButtonList[10], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_savePath = True;
            } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_startPlay = False;
                sdl->flagList.flag_autoMode = False;
                sdl->flagList.flag_menuButton = True;
                delete_grids(sdl->c);
                break;
            } else if (isButtonClicked(&sdl->ButtonList[timeSetButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_setTime = True;
            } else if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True) {
                changeGridStateOnClick(sdl->c, sdl->mouseX, sdl->mouseY);
                break;
            } else if (isButtonClicked(&sdl->ButtonList[resetGridButtonNum], sdl->mouseX, sdl->mouseY)) {
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    for (int i = 0; i < sdl->c->rows; i++) {
                        for (int j = 0; j < sdl->c->cols; j++) {
                            set_state(sdl->c, i, j, Dead);
                        }
                    }
                    break;
                }
            }

            // 文字输入的处理
        } else if (sdl->flagList.input_mode == True && sdl->e.type == SDL_KEYDOWN) {

            // 处理行 列 概率的输入
            if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
                // 检查输入的是数字
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符
                printf("用户输入了: %s\n", sdl->inputBuffer);
                if (sdl->flagList.input_rows == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->rows);
                    printf("Current row:%d\n", sdl->c->rows);
                } else if (sdl->flagList.input_cols == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->cols);
                    printf("Current Col:%d\n", sdl->c->cols);
                } else if (sdl->flagList.input_probability == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->probability);
                    printf("%d", sdl->c->probability);
                }
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    int temp_probability = sdl->c->probability;
                    sdl->c = update_conway(sdl->c->rows, sdl->c->cols);
                    sdl->c->probability = temp_probability;
                    if (sdl->c->probability > 0 && sdl->c->probability <= 100) {
                        init_random(sdl->c);
                    }
                }
            }

            SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色

            if (sdl->flagList.input_rows == True) {
                renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomInputBox_rowsNum].rect.x + 10,
                           sdl->ButtonList[init_randomInputBox_rowsNum].rect.y + 15);
            } else if (sdl->flagList.input_cols == True) {
                renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomInputBox_colsNum].rect.x + 10,
                           sdl->ButtonList[init_randomInputBox_colsNum].rect.y + 15);
            } else if (sdl->flagList.input_probability == True) {
                renderText(sdl->renderer, sdl->FontList[2], sdl->inputBuffer, textColor, sdl->ButtonList[init_randomButton_proNum].rect.x + 10,
                           sdl->ButtonList[init_randomButton_proNum].rect.y + 15);
            }

            SDL_RenderPresent(sdl->renderer); // 更新显示的内容

            // 处理文件名的输入
        } else if ((sdl->flagList.flag_importPath == True || sdl->flagList.flag_savePath == True) && sdl->e.type == SDL_KEYDOWN) {
            SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
            TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

            if (sdl->e.key.keysym.sym != SDLK_LALT) {
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符,按alt输入字母
            }
            if (sdl->e.key.keysym.sym == SDLK_0) {
                sdl->inputBuffer[0] = '\0';
            } else if (sdl->e.key.keysym.sym == SDLK_RETURN) {
                if (sdl->flagList.flag_importPath == True) {
                    sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
                    Conway *temp_c;
                    temp_c = new_conway_from_file(sdl->c, sdl->inputBuffer);
                    if (temp_c != NULL) {
                        sdl->c = new_conway_from_file(sdl->c, sdl->inputBuffer);
                    } else {
                        updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                            "Warning:Invalid Path!");
                    }
                    sdl->flagList.flag_importPath = False;
                    break;
                } else if (sdl->flagList.flag_savePath == True) {
                    sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
                    save_conway(sdl->c, sdl->inputBuffer);
                    sdl->flagList.flag_savePath = False;
                    break;
                }
                memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
            }
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

            // 设置时间
        } else if (sdl->flagList.flag_setTime == True && sdl->e.type == SDL_KEYDOWN) {
            if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
                // 检查输入的是数字
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符
                printf("用户输入了: %s\n", sdl->inputBuffer);
                sscanf(sdl->inputBuffer, "%d", &sdl->autoTime);
                if (sdl->autoTime < 80) {
                    sdl->autoTime = 80;
                }
            }
            TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);
            renderText(sdl->renderer, pathFont, sdl->inputBuffer, sdl->colorList[0], sdl->ButtonList[timeSetButtonNum].rect.x + 10,
                       sdl->ButtonList[timeSetButtonNum].rect.y + 15);
        }

        SDL_RenderPresent(sdl->renderer);
    }
}

void Special_mode_choice(SDL_resource *sdl) {
    int lastHoveredRow = -1;
    int lastHoveredCol = -1;
    while (SDL_WaitEvent(&sdl->e) != 0) {
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);

        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startSpecial = False;
            sdl->flagList.flag_mainGame = False;
            break;

            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
            sdl->inputPos = 0;
            memset(sdl->inputBuffer, 0, sizeof(sdl->inputBuffer));
            sdl->flagList.input_mode = False;
            if (isButtonClicked(&sdl->ButtonList[nextGenerationButtonNum], sdl->mouseX, sdl->mouseY)) {
                next_generation_special(sdl->c);
                sdl->c->grid_feature.count_turn += 1;
                if (sdl->c->rows <= 0 || sdl->c->cols <= 0) {
                    updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                        "Warning:Grids haven't been initialized!");
                    break;
                }
                break; // 重新绘制
            } else if (isButtonClicked(&sdl->ButtonList[init_randomButtonNum], sdl->mouseX, sdl->mouseY)) {
                printf("%d\n", sdl->c->probability);
                int temp_probability = sdl->c->probability;
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    sdl->c = new_conway_special(sdl->c->rows, sdl->c->cols);
                    sdl->c->probability = temp_probability;
                    printf("%d\n", temp_probability);
                    init_random_special(sdl->c);
                } else if (sdl->c->rows == 0 || sdl->c->cols == 0) {
                    sdl->c->rows = 20;
                    sdl->c->cols = 20;
                    sdl->c = new_conway_special(20, 20);
                    updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                        "Warning:Invalid Rows/Cols,using default setting!");
                    init_random_special(sdl->c);
                }
                break; // 重新绘制
            } else if (isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY) ||
                       isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY) ||
                       isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.input_mode = True;
                sdl->flagList.input_rows =
                    isButtonClicked(&sdl->ButtonList[init_randomInputBox_rowsNum], sdl->mouseX, sdl->mouseY); // 防止两个一起变成真
                sdl->flagList.input_cols = isButtonClicked(&sdl->ButtonList[init_randomInputBox_colsNum], sdl->mouseX, sdl->mouseY);
                sdl->flagList.input_probability = isButtonClicked(&sdl->ButtonList[init_randomButton_proNum], sdl->mouseX, sdl->mouseY);
            } else if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode_special = True;
                break;
            } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_startSpecial = False;
                sdl->flagList.flag_autoMode_special = False;
                sdl->flagList.flag_menuButton = True;
                delete_special_grids(sdl->c);
                break;
            } else if (isButtonClicked(&sdl->ButtonList[timeSetButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_setTime = True;

            } else if (isButtonClicked(&sdl->ButtonList[importPathButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_importPath = True;
            } else if (isButtonClicked(&sdl->ButtonList[savePathButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_savePath = True;
            } else if (isGridClicked(sdl->c, sdl->mouseX, sdl->mouseY) == True && sdl->flagList.flag_editGrid == True) {
                changeGridStateONCLick_special(sdl->c, sdl->mouseX, sdl->mouseY);
                draw_special_sdlGrids(sdl->renderer, sdl->c);
                SDL_RenderPresent(sdl->renderer);
            } else if (isButtonClicked(&sdl->ButtonList[resetGridButtonNum], sdl->mouseX, sdl->mouseY)) {
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    for (int i = 0; i < sdl->c->rows; i++) {
                        for (int j = 0; j < sdl->c->cols; j++) {
                            set_state_special(sdl->c, i, j, new_cell_setting());
                        }
                    }
                    sdl->c->grid_feature.count_turn = 0;
                    break;
                }
            } else if (isButtonClicked(&sdl->ButtonList[editModeButtonNum], sdl->mouseX, sdl->mouseY)) {
                if (sdl->flagList.flag_editGrid == False) {
                    sdl->flagList.flag_editGrid = True;
                    renderText(sdl->renderer, sdl->FontList[2], "Disable edit on click", (SDL_Color){255, 0, 0},
                               sdl->ButtonList[editModeButtonNum].rect.x + 18, sdl->ButtonList[editModeButtonNum].rect.y + 8);
                    updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                        "Yor are editing grids on click!");
                } else {
                    sdl->flagList.flag_editGrid = False;
                }
                break;
            }

        } else if (sdl->flagList.input_mode == True && sdl->e.type == SDL_KEYDOWN) {

            if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
                // 检查输入的是数字
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符
                printf("用户输入了: %s\n", sdl->inputBuffer);
                if (sdl->flagList.input_rows == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->rows);
                } else if (sdl->flagList.input_cols == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->cols);
                } else if (sdl->flagList.input_probability == True) {
                    sscanf(sdl->inputBuffer, "%d", &sdl->c->probability);
                    printf("%d", sdl->c->probability);
                }
                if (sdl->c->rows > 0 && sdl->c->cols > 0) {
                    int temp_probability = sdl->c->probability;
                    sdl->c = new_conway_special(sdl->c->rows, sdl->c->cols);
                    sdl->c->probability = temp_probability;
                    if (sdl->c->probability > 0 && sdl->c->probability <= 100) {
                        init_random_special(sdl->c);
                    }
                }

            } else if (sdl->e.key.keysym.sym == SDLK_RETURN) {

                printf("用户输入了: %s\n", sdl->inputBuffer);
            }
            // 使用renderText函数绘制inputBuffer的内容
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
        } else if (sdl->flagList.flag_setTime == True && sdl->e.type == SDL_KEYDOWN) {
            if (sdl->e.key.keysym.sym >= SDLK_0 && sdl->e.key.keysym.sym <= SDLK_9) {
                // 检查输入的是数字
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符
                printf("用户输入了: %s\n", sdl->inputBuffer);
                sscanf(sdl->inputBuffer, "%d I", &sdl->autoTime);
                if (sdl->autoTime < 80) {
                    sdl->autoTime = 80;
                }
            }
            TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

            renderText(sdl->renderer, pathFont, sdl->inputBuffer, sdl->colorList[0], sdl->ButtonList[timeSetButtonNum].rect.x + 10,
                       sdl->ButtonList[timeSetButtonNum].rect.y + 15);
            SDL_RenderPresent(sdl->renderer);
        } else if ((sdl->flagList.flag_importPath == True || sdl->flagList.flag_savePath == True) && sdl->e.type == SDL_KEYDOWN) {
            SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
            TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

            if (sdl->e.key.keysym.sym != SDLK_LALT) {
                sdl->inputBuffer[sdl->inputPos] = (char)sdl->e.key.keysym.sym;
                sdl->inputPos++;
                sdl->inputBuffer[sdl->inputPos] = '\0'; // 添加字符串结束符,按alt输入字母
            }
            if (sdl->e.key.keysym.sym == SDLK_0) {
                sdl->inputBuffer[0] = '\0';
            } else if (sdl->e.key.keysym.sym == SDLK_RETURN) {
                if (sdl->flagList.flag_importPath == True) {
                    sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
                    Conway *temp_c;
                    if ((temp_c = new_conway_from_file_special(sdl->c, sdl->inputBuffer)) != NULL) {
                        sdl->c = new_conway_from_file_special(sdl->c, sdl->inputBuffer);
                    } else {
                        updateCommandReport(sdl->commandReport_1, sdl->commandReport_2, sdl->commandReport_3, sdl->commandReport_4,
                                            "Warning:Invalid Path!");
                    }

                    sdl->flagList.flag_importPath = False;
                    // free(temp_c);
                    break;
                } else if (sdl->flagList.flag_savePath == True) {
                    sscanf(sdl->inputBuffer, "%s", &sdl->inputBuffer);
                    save_conway_special(sdl->c, sdl->inputBuffer);
                    sdl->flagList.flag_savePath = False;
                    break;
                }
                memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
            }
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

            // 设置时间
        } else if (sdl->e.type != SDL_MOUSEBUTTONDOWN && sdl->flagList.flag_editGrid == False) {
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
                    break;
                }
            } else {
                if (isWindowShown) {
                    // 这里关闭或隐藏信息窗口
                    isWindowShown = False;
                }
            }
        }
    }
}

void draw_normal_informationBoard(SDL_resource *sdl) {
    SDL_Color ButtontextColor = {35, 0, 255};
    sprintf(sdl->BasicGridInformation, "Rows:%d  Cols:%d  livingCells:%d  Auto Mode Pause time:%dms", sdl->c->rows, sdl->c->cols,
            count_living_cell_normal(sdl->c), sdl->autoTime);
    renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 45);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_1, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 60);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_2, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 75);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_3, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 90);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_4, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 105);

    // 初始化命令
    memset(sdl->commandReport_1, '\0', sizeof(sdl->commandReport_1));
    memset(sdl->commandReport_2, '\0', sizeof(sdl->commandReport_2));
    memset(sdl->commandReport_3, '\0', sizeof(sdl->commandReport_3));
    memset(sdl->commandReport_4, '\0', sizeof(sdl->commandReport_4));
}

// 信息和事件列表
void draw_special_informationBoard(SDL_resource *sdl) {
    SDL_Color ButtontextColor = {35, 0, 255};

    const char *temLevel[] = {"freezing", " cold", "cold", "cool", "cool", "normal", "warm", "warm", "hot", "hot", "extreme_hot"};
    const char *oxyLevel[] = {"extreme low", "low", "miner low", "normal", "abundant", "high", "extreme high"};
    const char *lightLevel[] = {"dark", "low", "low", "normal", "abundant", "high", "extreme high"};

    sprintf(sdl->BasicGridInformation, "Rows:%d  Cols:%d   Auto Mode Pause time:%dms", sdl->c->rows, sdl->c->cols, sdl->autoTime);
    sprintf(sdl->BasicGridInformation_2, "Producer number:%d  Consumer number:%d  Current turn:%d", count_living_producer(sdl->c),
            count_living_consumer(sdl->c), sdl->c->grid_feature.count_turn);
    if (sdl->c->rows > 0 && sdl->c->cols > 0) {
        sprintf(sdl->BasicGridInformation_3, "Temperature:%s  Oxygen:%s  Light:%s", temLevel[sdl->c->grid_feature.temperature],
                oxyLevel[sdl->c->grid_feature.oxygen_resource], lightLevel[sdl->c->grid_feature.light_resource]);
    }

    renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 45);
    renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation_2, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 60);
    renderText(sdl->renderer, sdl->FontList[3], sdl->BasicGridInformation_3, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 75);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_1, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 100);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_2, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 115);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_3, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 130);
    renderText(sdl->renderer, sdl->FontList[3], sdl->commandReport_4, ButtontextColor, sdl->ButtonList[informationBoardNum].rect.x + 10,
               sdl->ButtonList[informationBoardNum].rect.y + 145);

    showEventList(sdl->c, sdl->renderer, sdl->ButtonList[EventListNum]);

    // 初始化命令
    memset(sdl->commandReport_1, '\0', sizeof(sdl->commandReport_1));
    memset(sdl->commandReport_2, '\0', sizeof(sdl->commandReport_2));
    memset(sdl->commandReport_3, '\0', sizeof(sdl->commandReport_3));
    memset(sdl->commandReport_4, '\0', sizeof(sdl->commandReport_4));
}

void NormalAutoModeChoice(SDL_resource *sdl) {
    while (SDL_PollEvent(&sdl->e) != 0) { // 事件处理
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            sdl->flagList.flag_autoMode = False;
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode = False;
                break;
            } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode = False;
                sdl->flagList.flag_startPlay = False;
                sdl->flagList.flag_menuButton = True;
                delete_grids(sdl->c);
            }
        }
    }
}

void SpecialAutoModeChoice(SDL_resource *sdl) {
    while (SDL_PollEvent(&sdl->e) != 0) { // 事件处理
        SDL_GetMouseState(&sdl->mouseX, &sdl->mouseY);
        if (sdl->e.type == SDL_QUIT) {
            sdl->flagList.flag_autoMode_special = False;
            sdl->flagList.flag_startPlay = False;
            sdl->flagList.flag_mainGame = False;
            break;
            // 处理鼠标点击事件
        } else if (sdl->e.type == SDL_MOUSEBUTTONDOWN) {
            if (isButtonClicked(&sdl->ButtonList[startAutoModeButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode_special = False;
                break;
            } else if (isButtonClicked(&sdl->ButtonList[backToMenuButtonNum], sdl->mouseX, sdl->mouseY)) {
                sdl->flagList.flag_autoMode_special = False;
                sdl->flagList.flag_startSpecial = False;
                sdl->flagList.flag_menuButton = True;
                delete_special_grids(sdl->c);
            }
        } else if (sdl->e.type != SDL_MOUSEBUTTONDOWN && sdl->flagList.flag_editGrid == False) {
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
                    break;
                }
            } else {
                if (isWindowShown) {
                    // 这里关闭或隐藏信息窗口
                    isWindowShown = False;
                }
            }
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

// 释放Button中的字体
void cleanUpButton(Button *button) {
    if (button->font != NULL) {
        TTF_CloseFont(button->font);
        button->font = NULL;
    }
}

void free_all_resource(SDL_resource*sdl){
    SDL_DestroyTexture(sdl->texture);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    delete_grids(sdl->c);
    delete_special_grids(sdl->c);
    for (int i = 0; i < 20; ++i) {
        if (sdl->FontList[i] != NULL) {
            TTF_CloseFont(sdl->FontList[i]);
            sdl->FontList[i] = NULL;
        }
    }
    for (int i = 0; i < 40; ++i) {
        cleanUpButton(&sdl->ButtonList[i]);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    SDL_resource *sdl = (SDL_resource *)malloc(sizeof(SDL_resource));
    Conway *c = update_conway(0, 0);
    c->grid_feature.count_turn = 0;
    sdl->c = c;
    sdl->autoTime = 1000;
    memset(sdl->inputBuffer, '\0', sizeof(sdl->inputBuffer));
    sdl->inputPos = 0;

    if (initialize_sdl() == False) {
        delete_grids(c);
        free(sdl);
        return 1;
    }

    TTF_Init(); // 初始化SDL以及ttf（文字处理）
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

    memset(sdl->commandReport_1, '\0', sizeof(sdl->commandReport_1));
    memset(sdl->commandReport_2, '\0', sizeof(sdl->commandReport_2));
    memset(sdl->commandReport_3, '\0', sizeof(sdl->commandReport_3));
    memset(sdl->commandReport_4, '\0', sizeof(sdl->commandReport_4));
    memset(sdl->BasicGridInformation, '\0', sizeof(sdl->BasicGridInformation));
    memset(sdl->BasicGridInformation_2, '\0', sizeof(sdl->BasicGridInformation));
    memset(sdl->BasicGridInformation_3, '\0', sizeof(sdl->BasicGridInformation));

    SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);

    // 以上均为初始化各个变量

    while (sdl->flagList.flag_mainGame == True) {
        if (sdl->flagList.flag_menuButton == True) {
            showMenu(sdl);
        }

        if (sdl->flagList.flag_startPlay == True) {
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
                next_generation(c); // 更新Conway游戏的状态
                SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
                showButtons(sdl, 14); // 前14个按钮，用于普通模式
                changeButtonTextColor(sdl);
                draw_normal_informationBoard(sdl);
                draw_normal_sdlGrids(sdl->renderer, sdl->c);
                SDL_RenderPresent(sdl->renderer);
                NormalAutoModeChoice(sdl);
            }
            
        } else if (sdl->flagList.flag_startSpecial == True) {
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
    }
    free_all_resource(sdl);
    free(sdl);
    SDL_Quit();
    TTF_Quit();
    return 0;
}