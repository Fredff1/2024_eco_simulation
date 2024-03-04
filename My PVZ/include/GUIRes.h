#ifndef GUIRES_H
#define GUIRES_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <math.h>
#include <random>
#include <utility>
#define GRID_LENGTH 1800
#define HOME_LENGTH 466
#define HOME_LENGTH_PIXEL 466
#define GRID_WIDTH_PIXEL 148
#define GRID_HEIGHT_PIXEL 198
#define GRID_SKY_HEIGHT 75
#define WINDOW_HEIGHT 1200
#define WINDOW_WIDTH 1920
#define EXTRA_LENGTH (3*GRID_LENGTH)
#define MAGNIFY_RATE_WIDTH ((12*GRID_LENGTH)/WINDOW_WIDTH)
#define MAGNIFY_RATE_HEIGHT (1920/1038)



using std::unique_ptr;
using std::vector;
class Button;
class Level;

enum plantTypeInGrid{
    NoPlantType=0,
    PeaShooterType,
    SunflowerType,
};

enum ZombieType{
    NoZombieType=0,
    BasicZombieType=1,
    ConeHeadZombieType,
};

enum GameState{
    MAIN_MENU=0,
    INLEVEL,
    INLEVEL_PAUSE,
};

enum BackGroundState{
    DEFAULT=0,
    GRASSLANDBACKGROUND=1,
    MAINMENUBACKGROUND,
};

enum PlayerStateInLevel{
    DEFAULTPLAYERSTATE=0,
    CHOOSE_PLANT,
};

enum ZombieState{
    MOVING=0,
    ATTACKING,
    ICY_ZOMBIE,
    FROZEN_ZOMBIE,
    DYING,
    DYING_ANIME,
    DEAD_ZOMBIE,
};

enum BulletPierce{
    CANNOT_PIERCE,
    LIMITED_PIERCE,
    INFINITY_PIERCE,
    SPLASH_PIERCE,
};

enum SunshineType{
    NORMALSUNSHINE=0,
    HUGESUNSHINE=1,
    SMALLSUNSHINE,
};

enum SunshineState{
    FALLING_SUNSHINE,
    STABLE_SUNSHINE,
    MOVING_TO_SUNSHINEBANK,
    PLANT_GENERATED,
    DISAPPEARING,
};

enum ResourceType{
    SUNSHINE_RESOURCE,
    MONEY_RESOURCE,
};

enum ZombieArmor{
    NO_ARMOR=0,
    CONE_HEAD=1,
    BUCKET_HEAD,
};

enum SlotState{
        EMPTY_SLOT=0,
        AVAILABLE_SLOT,
        COOLDOWN_SLOT,
};

struct ZombieFeature{
    ZombieType type;
    int health;
    float speed;
    int damage;
    int critical_value;
    int attack_coolDown;

};

struct PlantFeature{
    plantTypeInGrid type;
    int health;
    int damage;
    int cost;
    float coolDown;
    int attackCoolDown;
};

class SDL_factory{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool initialize_sdl();
    SDL_Window *init_window(int length, int width);
    SDL_Renderer *render_window(SDL_Window *window);
    void RenderTextCentered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color);
    static std::map<ZombieType,std::map<std::string,SDL_Texture*>> zombieTexture;
    static std::map<plantTypeInGrid,std::map<std::string,SDL_Texture*>> plantTextures;
    static std::map<std::string,SDL_Texture*> backgroundTextures;
    static std::map<std::string,SDL_Texture*> buttonTexture;
    static std::map<std::string,SDL_Texture*> slotTextures;
    static std::map<int,TTF_Font* > basic_fonts;
    static std::map<ResourceType,std::map<std::string,SDL_Texture*>> resourceTextures;
    std::map<std::string,Button> menu_buttons;
    std::map<std::string,Button> level_buttons;
    SDL_Rect backgroundDestRect;
    SDL_Rect backgroundSrcRect;
    std::map<std::string,SDL_Rect> MenuButtonDestRect;
    int mouseX,mouseY;
    SDL_factory(){
        backgroundSrcRect.h=600,backgroundSrcRect.w=1038;
        backgroundSrcRect.x=0,backgroundSrcRect.y=0;
        backgroundDestRect.h=1200;
        backgroundDestRect.w=1920;
        backgroundDestRect.x=0;
        backgroundDestRect.y=0;
    }
    void changeLevelPauseState(const GameState state,GameState& stateInput);
        
};

class Button {
public:
    enum ButtonState {
        BUTTON_NORMAL,
        BUTTON_HOVER,
        BUTTON_PRESSED
    };
    SDL_Rect position; // 按钮在屏幕上的位置和尺寸
    SDL_Texture* normalTexture; // 正常状态的纹理
    SDL_Texture* hoverTexture; // 鼠标悬停状态的纹理
    ButtonState currentState; // 当前的按钮状态
    bool isAvail;
    char text[100];
    int text_size;

    Button(SDL_Rect rectInput={0,0,0,0},SDL_Texture* normal=nullptr,SDL_Texture* hover=nullptr,const char* textI="NULL") {
        currentState=BUTTON_NORMAL;
        position=rectInput;
        normalTexture=normal,hoverTexture=hover;
        isAvail=true;
        strcpy(text,textI);
        text_size=position.w/10;
    }

    void render(SDL_factory GUI_factory);
        
    void checkHover(int mouseX, int mouseY);

    bool isClicked(const int mouseX,const int mouseY);
};

std::pair<int,int> getPosFromGrid(const int rowPos,const int colPos,const BackGroundState background);
std::pair<int,int> getPosFromXY(const int x,const int y,const BackGroundState background);


#endif