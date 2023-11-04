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
    SDL_Texture *texture;
    // ...其他属性，例如标签、图标等
} Button;

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
Button createButton(int x, int y, int w, int h, SDL_Color color) {
    Button temp_button;
    temp_button.rect.x = x;
    temp_button.rect.y = y;
    temp_button.rect.w = w;
    temp_button.rect.h = h;
    temp_button.color = color;
    temp_button.isClicked = False;
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
    if (textTexture == NULL) {
        printf("Failed to create texture\n");
        return;
    }
    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
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

// 游戏界面的按钮的展示
void show_secondary_Button(Conway *c, SDL_Renderer *renderer, Button nextGenerationButton, Button init_randomButton, Button init_randomInputBox_rows,
                           Button init_randomInputBox_cols, Button init_randomButton_pro, Button startAutoModeButton) {

    // 画按钮

    // 最上面的按钮 下一世代
    SDL_SetRenderDrawColor(renderer, nextGenerationButton.color.r, nextGenerationButton.color.g, nextGenerationButton.color.b,
                           nextGenerationButton.color.a);
    SDL_RenderFillRect(renderer, &nextGenerationButton.rect);
    // 第二个按钮，生成格子
    SDL_SetRenderDrawColor(renderer, init_randomButton.color.r, init_randomButton.color.g, init_randomButton.color.b, init_randomButton.color.a);
    SDL_RenderFillRect(renderer, &init_randomButton.rect);
    // 下面三个与第二个按钮配套，用于设置行 列 概率
    SDL_SetRenderDrawColor(renderer, init_randomInputBox_rows.color.r, init_randomInputBox_rows.color.g, init_randomInputBox_rows.color.b,
                           init_randomInputBox_rows.color.a);
    SDL_RenderFillRect(renderer, &init_randomInputBox_rows.rect);

    SDL_SetRenderDrawColor(renderer, init_randomInputBox_cols.color.r, init_randomInputBox_cols.color.g, init_randomInputBox_cols.color.b,
                           init_randomInputBox_cols.color.a);
    SDL_RenderFillRect(renderer, &init_randomInputBox_cols.rect);

    SDL_SetRenderDrawColor(renderer, init_randomButton_pro.color.r, init_randomButton_pro.color.g, init_randomButton_pro.color.b,
                           init_randomButton_pro.color.a);
    SDL_RenderFillRect(renderer, &init_randomButton_pro.rect);

    // 用于自动模式的按钮
    SDL_SetRenderDrawColor(renderer, startAutoModeButton.color.r, startAutoModeButton.color.g, startAutoModeButton.color.b,
                           startAutoModeButton.color.a);
    SDL_RenderFillRect(renderer, &startAutoModeButton.rect);
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
void showCellInformationSpecial(Conway *c, SDL_Renderer *renderer, int mouseX, int mouseY) {
    int change_row = get_row_from_mouse(c, mouseX);
    int change_col = get_col_from_mouse(c, mouseY);

    TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
    Button float_information = createButton(mouseX, mouseY, 230, 100, (SDL_Color){50, 150, 225, 150});
    SDL_Color ButtontextColor = {0, 0, 0};
    SDL_SetRenderDrawColor(renderer, float_information.color.r, float_information.color.g, float_information.color.b, float_information.color.a);
    SDL_RenderFillRect(renderer, &float_information.rect);
    renderText(renderer, row_col_font, "Grid information:", ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 8);

    // 具体的信息
    char row_col_type_information[50];
    char age_hunger_repro[50];
    char tem[50];
    char oxy_or_light[50];
    const char *typeChar[10] = {"Dead", "Alive", "Producer", "Consumer", "Barrier"};
    const char *gene_tem[10] = {"Cold_resistant", "Cold_prefer", "Normal", "Warm_prefer", "Heat resistant"};
    const char *gene_light[10] = {"Dark_resistant", "Low_light prefer", "Normal", "High light prefer", "Extreme light resistant"};
    const char *gene_oxy[10] = {"Low oxygen resistant", "Low oxygen prefer", "Normal", "High oxygen prefer", "Extreme oxygen resistant"};
    sprintf(row_col_type_information, "Row:%d Col:%d Food:%d", change_row, change_col, get_food_state(c, change_row, change_col));

    sprintf(age_hunger_repro, "Age:%d Hunger:%d Type:%s", get_state_special(c, change_row, change_col).age,
            get_state_special(c, change_row, change_col).hunger, typeChar[get_state_special(c, change_row, change_col).type]);

    // 对于活细胞显示基因信息
    if (get_state_special(c, change_row, change_col).type != Dead && get_state_special(c, change_row, change_col).type != barrier) {
        sprintf(tem, "Temperature Preference:%s", gene_tem[get_state_special(c, change_row, change_col).gene_setting.temper_preference]);

    } else {
        sprintf(tem, "No gene for dead cell!");
    }
    // 生产者和消费者显示不一样
    if (get_state_special(c, change_row, change_col).type == living_producer) {
        sprintf(oxy_or_light, "Light Preference:%s", gene_oxy[get_state_special(c, change_row, change_col).gene_setting.light_preference]);

    } else if (get_state_special(c, change_row, change_col).type == living_consumer) {
        sprintf(oxy_or_light, "Oxygen Preference:%s", gene_light[get_state_special(c, change_row, change_col).gene_setting.oxy_preference]);

    } else {
        sprintf(oxy_or_light, "No gene for dead cell!");
    }

    renderText(renderer, row_col_font, row_col_type_information, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 26);
    renderText(renderer, row_col_font, age_hunger_repro, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 44);
    renderText(renderer, row_col_font, tem, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 62);
    renderText(renderer, row_col_font, oxy_or_light, ButtontextColor, float_information.rect.x + 8, float_information.rect.y + 80);

    SDL_Rect clipRect = {mouseX, mouseY, 200, 100};
    SDL_RenderSetClipRect(renderer, &clipRect);
    // ... 执行局部渲染操作 ...
    SDL_RenderPresent(renderer);
    SDL_RenderSetClipRect(renderer, NULL);
    TTF_CloseFont(row_col_font);
}


void showEventList(Conway *c, SDL_Renderer *renderer, Button EventList) {
    TTF_Font *eventFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
    char event_1[100], event_2[100], event_3[100], event_4[100], event_5[100];
    char event_6[100], event_7[100], event_8[100], event_9[100], event_10[100];
    char *eventChar[20] = {event_1, event_2, event_3, event_4, event_5, event_6, event_7, event_8, event_9, event_10};
    const char *eventList[20] = {"ice_age", "volcano_eruption", "drought", "warm_period", "blue_algal_bloom", "heat_wave"};

    for (int i = 0; i < 10; i++) {
        if (c->currentEvent[i] != none_event && c->event_remain_turn[i] != 0) {
            sprintf(eventChar[i], "Current Event %d: %s Remain Turn:%d", i + 1, eventList[(int)c->currentEvent[i]-1], c->event_remain_turn[i]);
            renderText(renderer, eventFont, eventChar[i], (SDL_Color){35, 0, 255}, 950, 765 + i * 15);
        }
    }
     renderText(renderer, eventFont, "Event List", (SDL_Color){35, 0, 255}, EventList.rect.x + 8, EventList.rect.y + 8);
}

// 主程序从这里开始
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (initialize_sdl() == False) {
        return 1;
    }

    TTF_Init(); // 初始化SDL以及ttf（文字处理）
    SDL_Window *window = init_window(1520, 1080);
    SDL_Renderer *renderer = render_window(window);
    SDL_SetWindowTitle(window, "Game of Life");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Bool flag_mainGame = True;
    Bool flag_menuButton = True;
    Bool flag_startPlay = False;
    Bool flag_startSpecial = False;
    Bool flag_autoMode = False;
    Bool flag_autoMode_special = False;

    char inputBuffer[40] = "";
    int inputPos = 0;
    int autoTime = 1000;
    Uint32 hoverStartTime = 0;

    Conway *c = update_conway(0, 0);
    int mouseX, mouseY;

    SDL_Texture *background = loadTexture(renderer, "./pic/ocean.jpg");

    while (flag_mainGame == True) {
        SDL_Event e;
        // 主菜单的三个按钮
        Button startButton = createButton(600, 450, 300, 100, (SDL_Color){50, 150, 210, 255});
        Button StartSpecialButton = createButton(600, 600, 300, 100, (SDL_Color){50, 150, 210, 255});
        Button quitButton = createButton(600, 750, 300, 100, (SDL_Color){50, 150, 210, 255});
        // 下面三项用于清除UI中的所有内容

        // 画游戏UI
        if (flag_menuButton == True) {

            // 绘制背景图
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);
            // 绘制主菜单三个按钮
            SDL_SetRenderDrawColor(renderer, startButton.color.r, startButton.color.g, startButton.color.b, startButton.color.a);
            SDL_RenderFillRect(renderer, &startButton.rect);
            SDL_RenderFillRect(renderer, &StartSpecialButton.rect);
            SDL_RenderFillRect(renderer, &quitButton.rect);

            // 文字渲染
            TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 25);
            TTF_Font *GameName_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 100);
            SDL_Color ButtontextColor = {35, 0, 255};
            renderText(renderer, GameName_font, "Game Of Life", ButtontextColor, 460, 180);
            renderText(renderer, row_col_font, "Start Normal Mode", ButtontextColor, startButton.rect.x + 36, startButton.rect.y + 28);
            renderText(renderer, row_col_font, "Start Special Mode", ButtontextColor, StartSpecialButton.rect.x + 36, StartSpecialButton.rect.y + 28);
            renderText(renderer, row_col_font, "Quit Game", ButtontextColor, quitButton.rect.x + 90, quitButton.rect.y + 28);
            SDL_RenderPresent(renderer);
        }

        while (SDL_PollEvent(&e) != 0) { // 事件处理
            if (e.type == SDL_QUIT) {
                flag_mainGame = False;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                if (isButtonClicked(&startButton, mouseX, mouseY)) {
                    flag_startPlay = True;
                    flag_menuButton = False;
                } else if (isButtonClicked(&StartSpecialButton, mouseX, mouseY)) {
                    flag_startSpecial = True;
                    flag_menuButton = False;
                    printf("yes");
                } else if (isButtonClicked(&quitButton, mouseX, mouseY)) {
                    flag_mainGame = False;
                    break;
                }
            }
        }

        if (flag_startPlay == True) {
            Bool input_mode = False;
            Bool input_rows = False;
            Bool input_cols = False;
            Bool input_probability = False;
            Bool flag_setTime = False;
            Bool flag_importPath = False;
            Bool flag_savePath = False;

            Button backToMenuButton = createButton(5, 5, 50, 30, (SDL_Color){50, 150, 210, 255});
            Button init_randomButton = createButton(950, 100, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button init_randomInputBox_rows = createButton(1130, 100, 60, 40, (SDL_Color){255,255,255,255});
            Button init_randomInputBox_cols = createButton(1210, 100, 60, 40, (SDL_Color){255,255,255,255});
            Button init_randomButton_pro = createButton(1290, 100, 120, 40, (SDL_Color){255,255,255,255});
            Button startAutoModeButton = createButton(950, 160, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button nextGenerationButton = createButton(950, 220, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button importChoiceButton = createButton(950, 280, 100, 40, (SDL_Color){50, 150, 210, 255});
            Button importPathButton = createButton(1080, 280, 240, 40, (SDL_Color){255,255,255,255});
            Button saveChoiceButton = createButton(950, 340, 100, 40, (SDL_Color){50, 150, 210, 255});
            Button savePathButton = createButton(1080, 340, 240, 40, (SDL_Color){255,255,255,255});
            Button timeSetButton = createButton(1150, 160, 120, 40, (SDL_Color){50, 150, 210, 255});
            Button resetGridButton = createButton(950, 400, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button informationBoard = createButton(950, 460, 450, 300, (SDL_Color){50, 150, 210, 255});

            SDL_RenderCopy(renderer, background, NULL, NULL);

            // 回到主菜单的按钮
            SDL_SetRenderDrawColor(renderer, backToMenuButton.color.r, backToMenuButton.color.g, backToMenuButton.color.b, backToMenuButton.color.a);
            SDL_RenderFillRect(renderer, &backToMenuButton.rect);

            show_secondary_Button(c, renderer, nextGenerationButton, init_randomButton, init_randomInputBox_rows, init_randomInputBox_cols,
                                  init_randomButton_pro, startAutoModeButton); // 可以添加更多

            SDL_RenderFillRect(renderer, &importChoiceButton.rect);

            SDL_RenderFillRect(renderer, &importPathButton.rect);

            SDL_RenderFillRect(renderer, &saveChoiceButton.rect);

            SDL_RenderFillRect(renderer, &savePathButton.rect);

            SDL_RenderFillRect(renderer, &timeSetButton.rect);

            SDL_RenderFillRect(renderer, &resetGridButton.rect);

            SDL_RenderFillRect(renderer, &informationBoard.rect);

            if (c->probability > 100 || c->probability < 0) {
                c->probability = 40;
            }

            // 按钮上的文字
            TTF_Font *Button_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 15);
            SDL_Color ButtontextColor = {35, 0, 255};
            renderText(renderer, Button_font, "Menu", ButtontextColor, backToMenuButton.rect.x + 3, backToMenuButton.rect.y + 8);
            renderText(renderer, Button_font, "Note:You can click on the grid to change Grid state to Alive(Black)/Dead(White)", ButtontextColor, 950,
                       60);
            renderText(renderer, Button_font, "Rows:", ButtontextColor, init_randomInputBox_rows.rect.x, init_randomInputBox_rows.rect.y);
            renderText(renderer, Button_font, "Cols:", ButtontextColor, init_randomInputBox_cols.rect.x, init_randomInputBox_cols.rect.y);
            renderText(renderer, Button_font, "Probability:(%)", ButtontextColor, init_randomButton_pro.rect.x, init_randomButton_pro.rect.y);
            renderText(renderer, Button_font, "Init grids(Default 20*20)", ButtontextColor, init_randomButton.rect.x + 3,
                       init_randomButton.rect.y + 8);
            renderText(renderer, Button_font, "Next Generation", ButtontextColor, nextGenerationButton.rect.x + 26, nextGenerationButton.rect.y + 8);
            renderText(renderer, Button_font, "start/stop AutoMode", ButtontextColor, startAutoModeButton.rect.x + 13,
                       startAutoModeButton.rect.y + 8);
            renderText(renderer, Button_font, "Load File", ButtontextColor, importChoiceButton.rect.x + 18, importChoiceButton.rect.y + 8);
            renderText(renderer, Button_font, "Save Grids", ButtontextColor, saveChoiceButton.rect.x + 14, saveChoiceButton.rect.y + 8);
            renderText(renderer, Button_font, "PauseTime(ms):", ButtontextColor, timeSetButton.rect.x, timeSetButton.rect.y);
            renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, importPathButton.rect.x + 10,
                       importPathButton.rect.y);

            renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, savePathButton.rect.x + 10,
                       savePathButton.rect.y);

            renderText(renderer, Button_font, "Reset all cells dead", ButtontextColor, resetGridButton.rect.x + 18, resetGridButton.rect.y + 8);

            renderText(renderer, Button_font, "Grid information:", ButtontextColor, informationBoard.rect.x + 8, informationBoard.rect.y + 8);
            // 画图片

            // 画格点
            draw_normal_sdlGrids(renderer, c);

            // 显示命令面板的指令
            TTF_Font *informationFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
            static char BasicGridInformation[100];
            static char commandReport_1[100];
            static char commandReport_2[100];
            static char commandReport_3[100];
            static char commandReport_4[100];

            sprintf(BasicGridInformation, "Rows:%d  Cols:%d  livingCells:%d  Auto Mode Pause time:%dms", c->rows, c->cols,
                    count_living_cell_normal(c), autoTime);
            renderText(renderer, informationFont, BasicGridInformation, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 45);
            renderText(renderer, informationFont, commandReport_1, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 60);
            renderText(renderer, informationFont, commandReport_2, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 75);
            renderText(renderer, informationFont, commandReport_3, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 90);
            renderText(renderer, informationFont, commandReport_4, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 105);

            SDL_RenderPresent(renderer);
            // 更新屏幕

            // 初始化命令
            memset(commandReport_1, '\0', sizeof(commandReport_1));
            memset(commandReport_2, '\0', sizeof(commandReport_2));
            memset(commandReport_3, '\0', sizeof(commandReport_3));
            memset(commandReport_4, '\0', sizeof(commandReport_4));

            while (SDL_WaitEvent(&e) != 0) { // 事件处理
                TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
                if (e.type == SDL_QUIT) {
                    flag_startPlay = False;
                    flag_mainGame = False;
                    break;
                    // 处理鼠标点击事件
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&mouseX, &mouseY);
                    inputPos = 0;
                    memset(inputBuffer, 0, sizeof(inputBuffer));
                    input_mode = False;
                    if (isButtonClicked(&nextGenerationButton, mouseX, mouseY)) {
                        if (c->rows <= 0 || c->cols <= 0) {
                            updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4,
                                                "Warning:Grids haven't been initialized!");
                            break;
                        }
                        next_generation(c);
                        break; // 重新绘制
                    } else if (isButtonClicked(&init_randomButton, mouseX, mouseY)) {
                        printf("%d\n", c->probability);
                        int temp_probability = c->probability;
                        if (c->rows > 0 && c->cols > 0) {
                            c = update_conway(c->rows, c->cols);
                            c->probability = temp_probability;
                            printf("%d\n", temp_probability);
                            init_random(c);
                        } else if (c->rows == 0 || c->cols == 0) {
                            c->rows = 20;
                            c->cols = 20;
                            c = update_conway(c->rows, c->cols);
                            updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4,
                                                "Warning:Invalid Rows/Cols,using default setting!");
                            init_random(c);
                        }
                        break; // 重新绘制
                    } else if (isButtonClicked(&init_randomInputBox_rows, mouseX, mouseY) ||
                               isButtonClicked(&init_randomInputBox_cols, mouseX, mouseY) ||
                               isButtonClicked(&init_randomButton_pro, mouseX, mouseY)) {
                        input_mode = True;
                        input_rows = isButtonClicked(&init_randomInputBox_rows, mouseX, mouseY);
                        input_cols = isButtonClicked(&init_randomInputBox_cols, mouseX, mouseY);
                        input_probability = isButtonClicked(&init_randomButton_pro, mouseX, mouseY);

                    } else if (isButtonClicked(&startAutoModeButton, mouseX, mouseY)) {
                        flag_autoMode = True;
                        break; // auto 的循环在下面，所以可以break;
                    } else if (isButtonClicked(&importPathButton, mouseX, mouseY)) {
                        flag_importPath = True;
                    } else if (isButtonClicked(&savePathButton, mouseX, mouseY)) {
                        flag_savePath = True;
                    } else if (isButtonClicked(&backToMenuButton, mouseX, mouseY)) {
                        flag_startPlay = False;
                        flag_autoMode = False;
                        flag_menuButton = True;
                        delete_grids(c);
                        break;
                    } else if (isButtonClicked(&timeSetButton, mouseX, mouseY)) {
                        flag_setTime = True;
                    } else if (isGridClicked(c, mouseX, mouseY) == True) {
                        changeGridStateOnClick(c, mouseX, mouseY);
                        break;
                    } else if (isButtonClicked(&resetGridButton, mouseX, mouseY)) {
                        if (c->rows > 0 && c->cols > 0) {
                            for (int i = 0; i < c->rows; i++) {
                                for (int j = 0; j < c->cols; j++) {
                                    set_state(c, i, j, Dead);
                                }
                            }
                            break;
                        }
                    }

                    // 文字输入的处理
                } else if (input_mode == True && e.type == SDL_KEYDOWN) {

                    // 处理行 列 概率的输入
                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                        // 检查输入的是数字
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符
                        printf("用户输入了: %s\n", inputBuffer);
                        if (input_rows == True) {
                            sscanf(inputBuffer, "%d", &c->rows);
                        } else if (input_cols == True) {
                            sscanf(inputBuffer, "%d", &c->cols);
                        } else if (input_probability == True) {
                            sscanf(inputBuffer, "%d", &c->probability);
                            printf("%d", c->probability);
                        }
                        if (c->rows > 0 && c->cols > 0) {
                            int temp_probability = c->probability;
                            c = update_conway(c->rows, c->cols);
                            c->probability = temp_probability;
                            if (c->probability > 0 && c->probability <= 100) {
                                init_random(c);
                            }
                        }
                    }

                    SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色

                    if (input_rows == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomInputBox_rows.rect.x + 10,
                                   init_randomInputBox_rows.rect.y + 15);
                    } else if (input_cols == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomInputBox_cols.rect.x + 10,
                                   init_randomInputBox_cols.rect.y + 15);
                    } else if (input_probability == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomButton_pro.rect.x + 10,
                                   init_randomButton_pro.rect.y + 15);
                    }

                    SDL_RenderPresent(renderer); // 更新显示的内容

                    // 处理文件名的输入
                } else if ((flag_importPath == True || flag_savePath == True) && e.type == SDL_KEYDOWN) {
                    SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
                    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

                    if (e.key.keysym.sym != SDLK_LALT) {
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符,按alt输入字母
                    }
                    if (e.key.keysym.sym == SDLK_0) {
                        inputBuffer[0] = '\0';
                    } else if (e.key.keysym.sym == SDLK_RETURN) {
                        if (flag_importPath == True) {
                            sscanf(inputBuffer, "%s", &inputBuffer);
                            Conway *temp_c;
                            temp_c = new_conway_from_file(c, inputBuffer);
                            if (temp_c != NULL) {
                                c = new_conway_from_file(c, inputBuffer);
                            } else {
                                updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4, "Warning:Invalid Path!");
                            }
                            flag_importPath = False;
                            break;
                        } else if (flag_savePath == True) {
                            sscanf(inputBuffer, "%s", &inputBuffer);
                            save_conway(c, inputBuffer);
                            flag_savePath = False;
                            break;
                        }
                        memset(inputBuffer, '\0', sizeof(inputBuffer));
                    }
                    if (flag_importPath == True) {
                        renderText(renderer, pathFont, inputBuffer, textColor, importPathButton.rect.x + 10, importPathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    } else if (flag_savePath == True) {
                        renderText(renderer, pathFont, inputBuffer, textColor, savePathButton.rect.x + 10, savePathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    } else {
                        renderText(renderer, pathFont, inputBuffer, textColor, importPathButton.rect.x + 10, importPathButton.rect.y + 15);
                        renderText(renderer, pathFont, inputBuffer, textColor, savePathButton.rect.x + 10, savePathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    }

                    // 设置时间
                } else if (flag_setTime == True && e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                        // 检查输入的是数字
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符
                        printf("用户输入了: %s\n", inputBuffer);
                        sscanf(inputBuffer, "%d", &autoTime);
                        if (autoTime < 80) {
                            autoTime = 80;
                        }
                    }
                    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);
                    renderText(renderer, pathFont, inputBuffer, ButtontextColor, timeSetButton.rect.x + 10, timeSetButton.rect.y + 15);
                }

                SDL_RenderPresent(renderer);
            }

            // 自动普通模式
            while (flag_autoMode == True) {
                SDL_RenderClear(renderer); // 清除旧的内容
                SDL_Delay(autoTime);

                next_generation(c); // 更新Conway游戏的状态

                // 背景图片
                SDL_RenderCopy(renderer, background, NULL, NULL);

                SDL_SetRenderDrawColor(renderer, backToMenuButton.color.r, backToMenuButton.color.g, backToMenuButton.color.b,
                                       backToMenuButton.color.a);
                SDL_RenderFillRect(renderer, &backToMenuButton.rect);

                show_secondary_Button(c, renderer, nextGenerationButton, init_randomButton, init_randomInputBox_rows, init_randomInputBox_cols,
                                      init_randomButton_pro, startAutoModeButton); // 可以添加更多

                SDL_RenderFillRect(renderer, &importChoiceButton.rect);

                SDL_RenderFillRect(renderer, &importPathButton.rect);

                SDL_RenderFillRect(renderer, &saveChoiceButton.rect);

                SDL_RenderFillRect(renderer, &savePathButton.rect);

                SDL_RenderFillRect(renderer, &timeSetButton.rect);

                SDL_RenderFillRect(renderer, &resetGridButton.rect);

                SDL_RenderFillRect(renderer, &informationBoard.rect);

                TTF_Font *Button_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 15);
                SDL_Color ButtontextColor = {255, 0, 0};
                SDL_Color ButtontextColor_autoControl = {35, 0, 255};
                renderText(renderer, Button_font, "Menu", ButtontextColor_autoControl, backToMenuButton.rect.x + 3, backToMenuButton.rect.y + 8);

                renderText(renderer, Button_font, "Rows:", ButtontextColor, init_randomInputBox_rows.rect.x, init_randomInputBox_rows.rect.y);
                renderText(renderer, Button_font, "Cols:", ButtontextColor, init_randomInputBox_cols.rect.x, init_randomInputBox_cols.rect.y);
                renderText(renderer, Button_font, "Probability:(%)", ButtontextColor, init_randomButton_pro.rect.x, init_randomButton_pro.rect.y);
                renderText(renderer, Button_font, "Init grids(Default 20*20)", ButtontextColor, init_randomButton.rect.x + 3,
                           init_randomButton.rect.y + 8);
                renderText(renderer, Button_font, "Next Generation", ButtontextColor, nextGenerationButton.rect.x + 26,
                           nextGenerationButton.rect.y + 8);
                renderText(renderer, Button_font, "start/stop AutoMode", ButtontextColor_autoControl, startAutoModeButton.rect.x + 13,
                           startAutoModeButton.rect.y + 8);
                renderText(renderer, Button_font, "Load File", ButtontextColor, importChoiceButton.rect.x + 18, importChoiceButton.rect.y + 8);
                renderText(renderer, Button_font, "Save Grids", ButtontextColor, saveChoiceButton.rect.x + 14, saveChoiceButton.rect.y + 8);
                renderText(renderer, Button_font, "PauseTime(ms):", ButtontextColor, timeSetButton.rect.x, timeSetButton.rect.y);
                renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, importPathButton.rect.x + 10,
                           importPathButton.rect.y);

                renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, savePathButton.rect.x + 10,
                           savePathButton.rect.y);

                renderText(renderer, Button_font, "Reset all cells dead", ButtontextColor, resetGridButton.rect.x + 18, resetGridButton.rect.y + 8);

                renderText(renderer, Button_font, "Grid information:", ButtontextColor_autoControl, informationBoard.rect.x + 8,
                           informationBoard.rect.y + 8);

                // 画图片
                draw_normal_sdlGrids(renderer, c);

                sprintf(BasicGridInformation, "Rows:%d  Cols:%d  livingCells:%d  Auto Mode Pause time:%dms", c->rows, c->cols,
                        count_living_cell_normal(c), autoTime);
                renderText(renderer, informationFont, BasicGridInformation, ButtontextColor_autoControl, informationBoard.rect.x + 10,
                           informationBoard.rect.y + 45);

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&e) != 0) { // 事件处理
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (e.type == SDL_QUIT) {
                        flag_startPlay = False;
                        flag_mainGame = False;
                        flag_autoMode = False;
                        break;
                        // 处理鼠标点击事件
                    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                        if (isButtonClicked(&startAutoModeButton, mouseX, mouseY)) {
                            flag_autoMode = False;
                            break;
                        } else if (isButtonClicked(&backToMenuButton, mouseX, mouseY)) {
                            flag_autoMode = False;
                            flag_startPlay = False;
                            flag_menuButton = True;
                            delete_grids(c);
                        }
                    }
                }
            }
        }

        // 特殊模式先不开发
        if (flag_startSpecial == True) {

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 设置为白色
            SDL_RenderClear(renderer);
            SDL_RenderFillRect(renderer, NULL); // 清除渲染目标

            Bool input_mode = False;
            Bool input_rows = False;
            Bool input_cols = False;
            Bool input_probability = False;
            Bool flag_setTime = False;
            Bool flag_importPath = False;
            Bool flag_savePath = False;
            static Bool flag_editGrid = False;

            Button backToMenuButton = createButton(5, 5, 50, 30, (SDL_Color){50, 150, 210, 255});
            Button init_randomButton = createButton(950, 100, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button init_randomInputBox_rows = createButton(1130, 100, 60, 40, (SDL_Color){255, 255, 255,255});
            Button init_randomInputBox_cols = createButton(1210, 100, 60, 40, (SDL_Color){255, 255, 255,255});
            Button init_randomButton_pro = createButton(1290, 100, 120, 40, (SDL_Color){255, 255, 255,255});
            Button startAutoModeButton = createButton(950, 160, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button nextGenerationButton = createButton(950, 220, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button importChoiceButton = createButton(950, 280, 100, 40, (SDL_Color){50, 150, 210, 255});
            Button importPathButton = createButton(1080, 280, 240, 40, (SDL_Color){255, 255, 255,255});
            Button saveChoiceButton = createButton(950, 340, 100, 40, (SDL_Color){50, 150, 210, 255});
            Button savePathButton = createButton(1080, 340, 240, 40, (SDL_Color){255, 255, 255,255});
            Button timeSetButton = createButton(1150, 160, 120, 40, (SDL_Color){50, 150, 210, 255});
            Button resetGridButton = createButton(1140, 400, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button informationBoard = createButton(950, 460, 450, 240, (SDL_Color){50, 150, 210, 255});
            Button editModeButton = createButton(950, 400, 170, 40, (SDL_Color){50, 150, 210, 255});
            Button EventList = createButton(950, 740, 450, 300, (SDL_Color){50, 150, 210, 255});

            // 背景图片
            SDL_RenderCopy(renderer, background, NULL, NULL);

            // 回到主菜单的按钮
            SDL_SetRenderDrawColor(renderer, backToMenuButton.color.r, backToMenuButton.color.g, backToMenuButton.color.b, backToMenuButton.color.a);
            SDL_RenderFillRect(renderer, &backToMenuButton.rect);

            show_secondary_Button(c, renderer, nextGenerationButton, init_randomButton, init_randomInputBox_rows, init_randomInputBox_cols,
                                  init_randomButton_pro, startAutoModeButton); // 可以添加更多

            SDL_RenderFillRect(renderer, &importChoiceButton.rect);

            SDL_RenderFillRect(renderer, &importPathButton.rect);

            SDL_RenderFillRect(renderer, &saveChoiceButton.rect);

            SDL_RenderFillRect(renderer, &savePathButton.rect);

            SDL_RenderFillRect(renderer, &timeSetButton.rect);

            SDL_RenderFillRect(renderer, &resetGridButton.rect);

            SDL_RenderFillRect(renderer, &informationBoard.rect);

            SDL_RenderFillRect(renderer, &editModeButton.rect);

            SDL_RenderFillRect(renderer, &EventList.rect);

            if (c->probability > 100 || c->probability < 0) {
                c->probability = 40;
            }

            // 按钮上的文字
            TTF_Font *Button_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 15);
            SDL_Color ButtontextColor={35,0,255};
            SDL_Color InputBoxColor={0,0,0};
            renderText(renderer, Button_font, "Menu", ButtontextColor, backToMenuButton.rect.x + 3, backToMenuButton.rect.y + 8);
            renderText(renderer, Button_font, "Note:You can click on the grid to change Grid state to Alive(Black)/Dead(White)", ButtontextColor, 950,
                       60);
            renderText(renderer, Button_font, "Rows:", ButtontextColor, init_randomInputBox_rows.rect.x, init_randomInputBox_rows.rect.y);
            renderText(renderer, Button_font, "Cols:", ButtontextColor, init_randomInputBox_cols.rect.x, init_randomInputBox_cols.rect.y);
            renderText(renderer, Button_font, "Probability:(%)", ButtontextColor, init_randomButton_pro.rect.x, init_randomButton_pro.rect.y);
            renderText(renderer, Button_font, "Init grids(Default 20*20)", ButtontextColor, init_randomButton.rect.x + 3,
                       init_randomButton.rect.y + 8);
            renderText(renderer, Button_font, "Next Generation", ButtontextColor, nextGenerationButton.rect.x + 26, nextGenerationButton.rect.y + 8);
            renderText(renderer, Button_font, "start/stop AutoMode", ButtontextColor, startAutoModeButton.rect.x + 13,
                       startAutoModeButton.rect.y + 8);
            renderText(renderer, Button_font, "Load File", ButtontextColor, importChoiceButton.rect.x + 18, importChoiceButton.rect.y + 8);
            renderText(renderer, Button_font, "Save Grids", ButtontextColor, saveChoiceButton.rect.x + 14, saveChoiceButton.rect.y + 8);
            renderText(renderer, Button_font, "PauseTime(ms):", ButtontextColor, timeSetButton.rect.x, timeSetButton.rect.y);
            renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, importPathButton.rect.x + 10,
                       importPathButton.rect.y);

            renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, savePathButton.rect.x + 10,
                       savePathButton.rect.y);

            renderText(renderer, Button_font, "Reset all cells dead", ButtontextColor, resetGridButton.rect.x + 18, resetGridButton.rect.y + 8);

            renderText(renderer, Button_font, "Grid information:", ButtontextColor, informationBoard.rect.x + 8, informationBoard.rect.y + 8);
            // 特别处理输入模式的情况
            if (flag_editGrid == False) {
                renderText(renderer, Button_font, "Enable edit on click", ButtontextColor, editModeButton.rect.x + 18, editModeButton.rect.y + 8);
            } else {
                renderText(renderer, Button_font, "Enable edit on click", (SDL_Color){0, 255, 0}, editModeButton.rect.x + 18,
                           editModeButton.rect.y + 8);
            }

           

            draw_special_sdlGrids(renderer, c);

            TTF_Font *row_col_font = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);

            TTF_Font *informationFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 14);
            static char BasicGridInformation[100];
            static char BasicGridInformation_2[100];
            static char BasicGridInformation_3[100];
            const char *temLevel[] = {"freezing", " cold", "cold", "cool", "cool", "normal", "warm", "warm", "hot", "hot", "extreme_hot"};
            const char *oxyLevel[] = {"extreme low", "low", "miner low", "normal", "abundant", "high", "extreme high"};
            const char *lightLevel[] = {"dark", "low", "low", "normal", "abundant", "high", "extreme high"};
            static char commandReport_1[100];
            static char commandReport_2[100];
            static char commandReport_3[100];
            static char commandReport_4[100];

            sprintf(BasicGridInformation, "Rows:%d  Cols:%d   Auto Mode Pause time:%dms", c->rows, c->cols, autoTime);
            sprintf(BasicGridInformation_2, "Producer number:%d  Consumer number:%d  Current turn:%d", count_living_producer(c),
                    count_living_consumer(c), c->grid_feature.count_turn);
            if (c->rows > 0 && c->cols > 0) {
                sprintf(BasicGridInformation_3, "Temperature:%s  Oxygen:%s  Light:%s", temLevel[c->grid_feature.temperature],
                        oxyLevel[c->grid_feature.oxygen_resource], lightLevel[c->grid_feature.light_resource]);
            }

            renderText(renderer, informationFont, BasicGridInformation, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 45);
            renderText(renderer, informationFont, BasicGridInformation_2, ButtontextColor, informationBoard.rect.x + 10,
                       informationBoard.rect.y + 60);
            renderText(renderer, informationFont, BasicGridInformation_3, ButtontextColor, informationBoard.rect.x + 10,
                       informationBoard.rect.y + 75);
            renderText(renderer, informationFont, commandReport_1, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 100);
            renderText(renderer, informationFont, commandReport_2, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 115);
            renderText(renderer, informationFont, commandReport_3, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 130);
            renderText(renderer, informationFont, commandReport_4, ButtontextColor, informationBoard.rect.x + 10, informationBoard.rect.y + 145);

            showEventList(c, renderer, EventList);

            SDL_RenderPresent(renderer);
            // 更新屏幕

            // 初始化命令
            memset(commandReport_1, '\0', sizeof(commandReport_1));
            memset(commandReport_2, '\0', sizeof(commandReport_2));
            memset(commandReport_3, '\0', sizeof(commandReport_3));
            memset(commandReport_4, '\0', sizeof(commandReport_4));
            int hoverStartTime = 0;
            int lastHoveredRow = -1;
            int lastHoveredCol = -1;
            while (SDL_WaitEvent(&e) != 0) {
                SDL_GetMouseState(&mouseX, &mouseY);

                if (e.type == SDL_QUIT) {
                    flag_startSpecial = False;
                    flag_mainGame = False;
                    break;

                    // 处理鼠标点击事件
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&mouseX, &mouseY);
                    inputPos = 0;
                    memset(inputBuffer, 0, sizeof(inputBuffer));
                    input_mode = False;
                    if (isButtonClicked(&nextGenerationButton, mouseX, mouseY)) {
                        next_generation_special(c);
                        c->grid_feature.count_turn += 1;
                        if (c->rows <= 0 || c->cols <= 0) {
                            updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4,
                                                "Warning:Grids haven't been initialized!");
                            break;
                        }
                        break; // 重新绘制
                    } else if (isButtonClicked(&init_randomButton, mouseX, mouseY)) {
                        printf("%d\n", c->probability);
                        int temp_probability = c->probability;
                        if (c->rows > 0 && c->cols > 0) {
                            c = new_conway_special(c->rows, c->cols);
                            c->probability = temp_probability;
                            printf("%d\n", temp_probability);
                            init_random_special(c);
                        } else if (c->rows == 0 || c->cols == 0) {
                            c->rows = 20;
                            c->cols = 20;
                            c = new_conway_special(c->rows, c->cols);
                            updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4,
                                                "Warning:Invalid Rows/Cols,using default setting!");
                            init_random_special(c);
                        }
                        break; // 重新绘制
                    } else if (isButtonClicked(&init_randomInputBox_rows, mouseX, mouseY) ||
                               isButtonClicked(&init_randomInputBox_cols, mouseX, mouseY) ||
                               isButtonClicked(&init_randomButton_pro, mouseX, mouseY)) {
                        input_mode = True;
                        input_rows = isButtonClicked(&init_randomInputBox_rows, mouseX, mouseY); // 防止两个一起变成真
                        input_cols = isButtonClicked(&init_randomInputBox_cols, mouseX, mouseY);
                        input_probability = isButtonClicked(&init_randomButton_pro, mouseX, mouseY);
                    } else if (isButtonClicked(&startAutoModeButton, mouseX, mouseY)) {
                        flag_autoMode_special = True;
                        break;
                    } else if (isButtonClicked(&backToMenuButton, mouseX, mouseY)) {
                        flag_startSpecial = False;
                        flag_autoMode_special = False;
                        flag_menuButton = True;
                        delete_special_grids(c);
                        break;
                    } else if (isButtonClicked(&timeSetButton, mouseX, mouseY)) {
                        flag_setTime = True;

                    } else if (isButtonClicked(&importPathButton, mouseX, mouseY)) {
                        flag_importPath = True;
                    } else if (isButtonClicked(&savePathButton, mouseX, mouseY)) {
                        flag_savePath = True;
                    } else if (isGridClicked(c, mouseX, mouseY) == True && flag_editGrid == True) {
                        changeGridStateONCLick_special(c, mouseX, mouseY);
                        draw_special_sdlGrids(renderer, c);
                        SDL_RenderPresent(renderer);
                    } else if (isButtonClicked(&resetGridButton, mouseX, mouseY)) {
                        if (c->rows > 0 && c->cols > 0) {
                            for (int i = 0; i < c->rows; i++) {
                                for (int j = 0; j < c->cols; j++) {
                                    set_state_special(c, i, j, new_cell_setting());
                                }
                            }
                            c->grid_feature.count_turn = 0;
                            break;
                        }
                    } else if (isButtonClicked(&editModeButton, mouseX, mouseY)) {
                        if (flag_editGrid == False) {
                            flag_editGrid = True;
                            updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4,
                                                "Yor are editing grids on click!");
                        } else {
                            flag_editGrid = False;
                        }
                        break;
                    }

                } else if (input_mode == True && e.type == SDL_KEYDOWN) {

                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                        // 检查输入的是数字
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符
                        printf("用户输入了: %s\n", inputBuffer);
                        if (input_rows == True) {
                            sscanf(inputBuffer, "%d", &c->rows);
                        } else if (input_cols == True) {
                            sscanf(inputBuffer, "%d", &c->cols);
                        } else if (input_probability == True) {
                            sscanf(inputBuffer, "%d", &c->probability);
                            printf("%d", c->probability);
                        }
                        if (c->rows > 0 && c->cols > 0) {
                            int temp_probability = c->probability;
                            c = new_conway_special(c->rows, c->cols);
                            c->probability = temp_probability;
                            if (c->probability > 0 && c->probability <= 100) {
                                init_random_special(c);
                            }
                        }

                    } else if (e.key.keysym.sym == SDLK_RETURN) {

                        printf("用户输入了: %s\n", inputBuffer);
                    }
                    // 使用renderText函数绘制inputBuffer的内容
                    SDL_Color textColor = {2, 200, 0};

                    if (input_rows == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomInputBox_rows.rect.x + 10,
                                   init_randomInputBox_rows.rect.y + 15);
                    } else if (input_cols == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomInputBox_cols.rect.x + 10,
                                   init_randomInputBox_cols.rect.y + 15);
                    } else if (input_probability == True) {
                        renderText(renderer, row_col_font, inputBuffer, textColor, init_randomButton_pro.rect.x + 10,
                                   init_randomButton_pro.rect.y + 15);
                    } else {
                        renderText(renderer, row_col_font, "Enter your path here:", textColor, init_randomButton_pro.rect.x + 10,
                                   init_randomButton_pro.rect.y + 15);
                    }

                    SDL_RenderPresent(renderer); // 更新显示的内容
                } else if (flag_setTime == True && e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                        // 检查输入的是数字
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符
                        printf("用户输入了: %s\n", inputBuffer);
                        sscanf(inputBuffer, "%d I", &autoTime);
                        if (autoTime < 80) {
                            autoTime = 80;
                        }
                    }
                    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

                    renderText(renderer, pathFont, inputBuffer, ButtontextColor, timeSetButton.rect.x + 10, timeSetButton.rect.y + 15);
                    SDL_RenderPresent(renderer);
                } else if ((flag_importPath == True || flag_savePath == True) && e.type == SDL_KEYDOWN) {
                    SDL_Color textColor = {2, 200, 0}; // 设置输入文本颜色
                    TTF_Font *pathFont = TTF_OpenFont("./Roboto/Roboto-Black.ttf", 12);

                    if (e.key.keysym.sym != SDLK_LALT) {
                        inputBuffer[inputPos] = (char)e.key.keysym.sym;
                        inputPos++;
                        inputBuffer[inputPos] = '\0'; // 添加字符串结束符,按alt输入字母
                    }
                    if (e.key.keysym.sym == SDLK_0) {
                        inputBuffer[0] = '\0';
                    } else if (e.key.keysym.sym == SDLK_RETURN) {
                        if (flag_importPath == True) {
                            sscanf(inputBuffer, "%s", &inputBuffer);
                            Conway *temp_c;
                            if ((temp_c=new_conway_from_file_special(c,inputBuffer)) !=NULL) {
                                c=new_conway_from_file_special(c,inputBuffer);
                            } else {
                                updateCommandReport(commandReport_1, commandReport_2, commandReport_3, commandReport_4, "Warning:Invalid Path!");
                            }

                            flag_importPath = False;
                           // free(temp_c);
                            break;
                        } else if (flag_savePath == True) {
                            sscanf(inputBuffer, "%s", &inputBuffer);
                            save_conway_special(c, inputBuffer);
                            flag_savePath = False;
                            break;
                        }
                        memset(inputBuffer, '\0', sizeof(inputBuffer));
                    }
                    if (flag_importPath == True) {
                        renderText(renderer, pathFont, inputBuffer, textColor, importPathButton.rect.x + 10, importPathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    } else if (flag_savePath == True) {
                        renderText(renderer, pathFont, inputBuffer, textColor, savePathButton.rect.x + 10, savePathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    } else {
                        renderText(renderer, pathFont, inputBuffer, textColor, importPathButton.rect.x + 10, importPathButton.rect.y + 15);
                        renderText(renderer, pathFont, inputBuffer, textColor, savePathButton.rect.x + 10, savePathButton.rect.y + 15);
                        SDL_RenderPresent(renderer);
                    }

                    // 设置时间
                } else if (e.type != SDL_MOUSEBUTTONDOWN && flag_editGrid == False) {
                    static Bool isWindowShown = False;
                    int currentRow = get_row_from_mouse(c, mouseX);
                    int currentCol = get_col_from_mouse(c, mouseY);

                    if (isGridClicked(c, mouseX, mouseY) == True) {
                        if ((currentRow != lastHoveredRow || currentCol != lastHoveredCol)) {
                            SDL_Delay(100);
                            showCellInformationSpecial(c, renderer, mouseX, mouseY);
                            isWindowShown = True; // 设置窗口为显示状态
                            lastHoveredRow = currentRow;
                            lastHoveredCol = currentCol;
                            while (SDL_WaitEvent(&e) == 0) {}
                            SDL_Delay(200);
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
            // 在这里处理information board

            TTF_CloseFont(row_col_font);
            SDL_RenderPresent(renderer);

            // 循环前打开字体，节省资源

            while (flag_autoMode_special == True) {
                c->grid_feature.count_turn += 1;
                SDL_Delay(autoTime); // 等待1秒

                SDL_RenderClear(renderer); // 清除旧的内容

                next_generation_special(c); // 更新Conway游戏的状态

                // 背景图片
                SDL_RenderCopy(renderer, background, NULL, NULL);

                SDL_SetRenderDrawColor(renderer, backToMenuButton.color.r, backToMenuButton.color.g, backToMenuButton.color.b,
                                       backToMenuButton.color.a);
                SDL_RenderFillRect(renderer, &backToMenuButton.rect);

                // 常用的按钮
                show_secondary_Button(c, renderer, nextGenerationButton, init_randomButton, init_randomInputBox_rows, init_randomInputBox_cols,
                                      init_randomButton_pro, startAutoModeButton); // 可以添加更多

                SDL_RenderFillRect(renderer, &importChoiceButton.rect);

                SDL_RenderFillRect(renderer, &importPathButton.rect);

                SDL_RenderFillRect(renderer, &saveChoiceButton.rect);

                SDL_RenderFillRect(renderer, &savePathButton.rect);

                SDL_RenderFillRect(renderer, &timeSetButton.rect);

                SDL_RenderFillRect(renderer, &resetGridButton.rect);

                SDL_RenderFillRect(renderer, &editModeButton.rect);

                SDL_RenderFillRect(renderer, &informationBoard.rect);

                SDL_RenderFillRect(renderer, &EventList.rect);

                // 绘制文字
                SDL_Color ButtontextColor_autoControl = {35, 0, 255};
                ButtontextColor = (SDL_Color){255, 0, 0};
                renderText(renderer, Button_font, "Menu", ButtontextColor_autoControl, backToMenuButton.rect.x + 3, backToMenuButton.rect.y + 8);

                renderText(renderer, Button_font, "Rows:", ButtontextColor, init_randomInputBox_rows.rect.x, init_randomInputBox_rows.rect.y);
                renderText(renderer, Button_font, "Cols:", ButtontextColor, init_randomInputBox_cols.rect.x, init_randomInputBox_cols.rect.y);
                renderText(renderer, Button_font, "Probability:(%)", ButtontextColor, init_randomButton_pro.rect.x, init_randomButton_pro.rect.y);
                renderText(renderer, Button_font, "Init grids(Default 20*20)", ButtontextColor, init_randomButton.rect.x + 3,
                           init_randomButton.rect.y + 8);
                renderText(renderer, Button_font, "Next Generation", ButtontextColor, nextGenerationButton.rect.x + 26,
                           nextGenerationButton.rect.y + 8);
                renderText(renderer, Button_font, "start/stop AutoMode", ButtontextColor_autoControl, startAutoModeButton.rect.x + 13,
                           startAutoModeButton.rect.y + 8);
                renderText(renderer, Button_font, "Load File", ButtontextColor, importChoiceButton.rect.x + 18, importChoiceButton.rect.y + 8);
                renderText(renderer, Button_font, "Save Grids", ButtontextColor, saveChoiceButton.rect.x + 14, saveChoiceButton.rect.y + 8);
                renderText(renderer, Button_font, "PauseTime(ms):", ButtontextColor, timeSetButton.rect.x, timeSetButton.rect.y);
                renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, importPathButton.rect.x + 10,
                           importPathButton.rect.y);

                renderText(renderer, Button_font, "Click to enter your path here:", (SDL_Color){2, 200, 0, 255}, savePathButton.rect.x + 10,
                           savePathButton.rect.y);

                renderText(renderer, Button_font, "Reset all cells dead", ButtontextColor, resetGridButton.rect.x + 18, resetGridButton.rect.y + 8);
                renderText(renderer, Button_font, "Grid information:", ButtontextColor_autoControl, informationBoard.rect.x + 8,
                           informationBoard.rect.y + 8);
                renderText(renderer, Button_font, "Enable edit on click", ButtontextColor, editModeButton.rect.x + 18, editModeButton.rect.y + 8);

               
                // 绘制格点
                draw_special_sdlGrids(renderer, c);

                // 打印信息
                sprintf(BasicGridInformation, "Rows:%d  Cols:%d   Auto Mode Pause time:%dms", c->rows, c->cols, autoTime);
                sprintf(BasicGridInformation_2, "Producer number:%d  Consumer number:%d  Current turn:%d", count_living_producer(c),
                        count_living_consumer(c), c->grid_feature.count_turn);
                if (c->rows > 0 && c->cols > 0) {
                    sprintf(BasicGridInformation_3, "Temperature:%s  Oxygen:%s  Light:%s", temLevel[c->grid_feature.temperature],
                            oxyLevel[c->grid_feature.oxygen_resource], lightLevel[c->grid_feature.light_resource]);
                }
                renderText(renderer, informationFont, BasicGridInformation, ButtontextColor_autoControl, informationBoard.rect.x + 10,
                           informationBoard.rect.y + 45);
                renderText(renderer, informationFont, BasicGridInformation_2, ButtontextColor_autoControl, informationBoard.rect.x + 10,
                           informationBoard.rect.y + 60);
                renderText(renderer, informationFont, BasicGridInformation_3, ButtontextColor_autoControl, informationBoard.rect.x + 10,
                           informationBoard.rect.y + 75);

                showEventList(c, renderer, EventList);

                // 展示
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&e) != 0) { // 事件处理
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (e.type == SDL_QUIT) {
                        flag_autoMode_special = False;
                        flag_startPlay = False;
                        flag_mainGame = False;
                        break;
                        // 处理鼠标点击事件
                    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                        if (isButtonClicked(&startAutoModeButton, mouseX, mouseY)) {
                            flag_autoMode_special = False;
                            break;
                        } else if (isButtonClicked(&backToMenuButton, mouseX, mouseY)) {
                            flag_autoMode_special = False;
                            flag_startSpecial = False;
                            flag_menuButton = True;
                            delete_special_grids(c);
                        }
                    }
                }
            }
        }
    }
    SDL_DestroyTexture(background);

    SDL_Quit();
    TTF_Quit();
    return 0;
}