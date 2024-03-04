#include "GUIRes.h"


std::map<ZombieType,std::map<std::string,SDL_Texture*>> SDL_factory::zombieTexture;
std::map<plantTypeInGrid,std::map<std::string,SDL_Texture*>> SDL_factory::plantTextures;
std::map<std::string,SDL_Texture*> SDL_factory::backgroundTextures;
std::map<std::string,SDL_Texture*> SDL_factory::buttonTexture;
std::map<std::string,SDL_Texture*> SDL_factory::slotTextures;
std::map<int,TTF_Font* > SDL_factory::basic_fonts;
std::map<ResourceType,std::map<std::string,SDL_Texture*>> SDL_factory::resourceTextures;

//接受范围row>=0,col>=1时在格点内
std::pair<int,int> getPosFromGrid(const int rowPos,const int colPos,const BackGroundState background){
    int x,y;
    switch(background){
        case GRASSLANDBACKGROUND:
        x=HOME_LENGTH_PIXEL+(colPos-1)*GRID_WIDTH_PIXEL;
        y = GRID_SKY_HEIGHT+rowPos*GRID_HEIGHT_PIXEL;
        break;
    }
    if(y<0){
        y=0;
    }
    return std::make_pair(x,y);
}

//-1 -1表示未得到合法的坐标,row:0-4,col:0-11
std::pair<int,int> getPosFromXY(const int x,const int y,const BackGroundState background){
    int row,col;
    if(x<=HOME_LENGTH_PIXEL||y<GRID_SKY_HEIGHT||y>=(GRID_SKY_HEIGHT+GRID_HEIGHT_PIXEL*5)){
        return std::make_pair(-1,-1);
    }
    switch(background){
        case GRASSLANDBACKGROUND:
        col=((x)-HOME_LENGTH_PIXEL)/GRID_WIDTH_PIXEL+1;
        row=((y)-GRID_SKY_HEIGHT*2)/GRID_HEIGHT_PIXEL+1;
        break;
    }
    
    return std::make_pair(row,col);
}


bool Button::isClicked(const int mouseX,const int mouseY){
    bool isClicked = mouseX >= position.x && mouseX <= position.x + position.w &&
                       mouseY >= position.y && mouseY <= position.y + position.h;
    return isClicked;
}

void Button::checkHover(int mouseX, int mouseY){
    // 检查鼠标是否在按钮范围内
        bool isHover = mouseX >= position.x && mouseX <= position.x + position.w &&
                       mouseY >= position.y && mouseY <= position.y + position.h;
        currentState = isHover ? BUTTON_HOVER : BUTTON_NORMAL;
}


void Button::render(SDL_factory GUI_factory){
    switch (currentState) {
            case BUTTON_NORMAL:
                SDL_RenderCopy(GUI_factory.renderer, normalTexture, nullptr, &position);
                
                break;
            case BUTTON_HOVER:
                SDL_RenderCopy(GUI_factory.renderer, hoverTexture, nullptr, &position);
                break;
            // 其他状态处理...
        }
    GUI_factory.RenderTextCentered(GUI_factory.renderer,GUI_factory.basic_fonts[text_size],text,position,{210,210,210});
}

 void SDL_factory::changeLevelPauseState(const GameState state,GameState& stateInput){
    level_buttons["level_option"].isAvail=!level_buttons["level_option"].isAvail;
    level_buttons["level_pause_backGame"].isAvail=!level_buttons["level_pause_backGame"].isAvail;
    level_buttons["level_pause_toMenu"].isAvail=!level_buttons["level_pause_toMenu"].isAvail;
    stateInput=state;
 }

 bool SDL_factory::initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! S  DL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

// 生成一个窗口
SDL_Window * SDL_factory::init_window(int length, int width) {
    SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length, width, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    return window;
}

// 渲染窗口的渲染器
SDL_Renderer * SDL_factory::render_window(SDL_Window *window) { // 渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

void SDL_factory::RenderTextCentered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color) {
    // 创建文字的表面
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == nullptr) {
        // 错误处理
        return;
    }

    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        // 错误处理
        SDL_FreeSurface(surface);
        return;
    }

    // 计算文本的位置以居中显示
    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect renderQuad = { rect.x + (rect.w - textWidth) / 2, rect.y + (rect.h - textHeight) / 2, textWidth, textHeight };

    // 渲染文本
    SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);

    // 清理
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
