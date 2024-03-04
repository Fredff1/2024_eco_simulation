#ifndef GAME_H
#define GAME_H
#include "Plant.h"
#include "Zombie.h"
#include "GUIRes.h"
#include <vector>
#include <memory>
#include <iostream>

class plantSlot;


using std::vector;
using std::unique_ptr;
using std::cout;
using std::cin;
using std::endl;










struct ZombieWave {
    std::vector<std::string> zombies; // 僵尸类型和行信息
    std::vector<int> zombie_row;
};

//存储关卡的信息
class Level {
public:
    int levelNumber;
    int maxWave;
    int sun;
    int sunGenerateCount;
    int chosenSlotNum;
    bool isPaused;
    bool isGridGenerated;
    bool isWave_started;
    vector<vector<GameGrid>> grid;
    std::vector<std::string> zombieTypes; // 此关卡中出现的僵尸类型
    std::map<int, ZombieWave> waves; // 每个波次的僵尸信息
    std::vector<plantSlot> slots;
    std::vector<Sunshine> sunshineList;
    std::string savePath;
    PlayerStateInLevel playerState;
    plantTypeInGrid chosenPlant;
    int zombie_wave_time_trigger;
    void loadLevel(const std::string& filePath);
    bool saveLevel();
    void setDefault();
    bool init_grid(int flag);//12列
    Level(){
        zombie_wave_time_trigger=0,sun=0,isGridGenerated=false;
        isPaused=false,chosenPlant=NoPlantType;
        savePath="./config/saved_game.txt",playerState=DEFAULTPLAYERSTATE;
        sunGenerateCount=0;
    }
    // 其他成员函数...
};

class GameGrid{
private:    
public:
    vector<unique_ptr<Plant>> plants;
    vector<unique_ptr<Zombie>> zombies;
    vector<unique_ptr<PlantBullet>> bullets;
    vector<Sunshine> grid_sunshine_temp_storage;
    plantTypeInGrid plantType;
    int rowPos;
    int colPos;
    GameGrid():zombies(),plantType(NoPlantType){
    }
     // 移动构造函数
    GameGrid(GameGrid&& other) noexcept = default;

    // 移动赋值运算符
    GameGrid& operator=(GameGrid&& other) noexcept = default;

    // 删除复制构造函数和复制赋值运算符
    GameGrid(const GameGrid&) = delete;
    GameGrid& operator=(const GameGrid&) = delete;
    ~GameGrid(){}
};

//游戏中的卡槽
class plantSlot{
public:
    
    SDL_Rect position; // 卡槽在屏幕上的位置和尺寸
    SDL_Texture* slotTexture; // 卡槽纹理
    std::map<std::string,SDL_Rect> positions;
    std::map<std::string,SDL_Texture*> slotTextures;
    SDL_Texture* plantTexture;
    //第一个是源矩形，第二个是目标矩形
    std::pair<SDL_Rect,SDL_Rect> textureOnCardRects;
    SDL_Rect cost_word_destRect;
    SDL_Color colorOfWord;
    int redColorRemainTime;
    SlotState state; // 卡槽当前状态
    plantTypeInGrid plantType;
    int plantCardCost;
    int cooldownTime; // 冷却时间（游戏刻）
    int cooldownCount;
    int Pos;
    plantSlot(SDL_factory GUI_factory,int posInput,plantTypeInGrid type_p);
    void setPlantTexture(SDL_factory GUI_factory);
    
    void render(SDL_Renderer* renderer);

    void renderPlantCard(SDL_factory& GUI_factory);
    //决定是否被选中
    bool is_selected(int mouseX, int mouseY);
    void updateSlotState();
};


class PlayerProperty{
public:    
    int maxSlot;
    int money;
    int start_sun;
    void writePlayerProperty();
    void readPlayerProperty();
    PlayerProperty():maxSlot(10),money(0),start_sun(50){}

};

class Game{
private:
    bool isRunning;
    int score;
    const int basicWave;
    int waveCount;
    int hugeWaveCount;
public:
    int flag;
   
    int FPS;
    bool startGeneratingWaves;
    int generated_zombie_number;
    int waveNumber;
    bool isInLevel;

    GameState gameState;
    Level playingLevel;
    std::string backGroundName;
    std::map<std::string,Level> storedLevel;
    
    PlantFactory plant_factory;
    ZombieFactory zombie_factory;
    SDL_Event e;
    SDL_factory GUI_factory;
    BackGroundState backGroundState;
    PlayerProperty playerProperty;

    Game(int hugeWaveInput):isRunning(true),score(0),basicWave(10),hugeWaveCount(hugeWaveInput),waveCount(0){
        startGeneratingWaves=false,isInLevel=false;
        generated_zombie_number=0,waveNumber=1,FPS=60;
        backGroundState=MAINMENUBACKGROUND;
    }
    void printGrid();
    
    //加载资源
    void loadResource();
    //主菜单
    void loadMainMenu();
    //开始level
    void InLevel(const std::string LevelInfo);
    bool setPlant(plantTypeInGrid plantType,int row,int col);
    //生成每一波僵尸
    void generateZombieWaves(const ZombieWave target_wave);
    //加载资源
    void loadMainGame(int targetLevel);
    //更新游戏状态
    void updateGameState();
    //主循环
    void mainGame();
    //更新化关卡内GUI
    void updateLevelGUI();
    //初始化level
    void init_level();
    //处理所有僵尸移动
    void zombiesMove();
    //更新僵尸的行动逻辑
    void updateZombieState();
    //处理僵尸变化格子
    void updateZombieAct();
    //处理僵尸的格子变化
    void updateZombiePos();
    //更新植物弹幕状态
    void updatePlantBulletAct();
    //更新植物弹幕的碰撞
    void updateBulletCollision();
    //更新植物弹幕的格子移动
    void updateBulletPos();
    //加载植物纹理
    void renderPlants();
    //植物的行动
    void updatePlantAct();
    //加载僵尸纹理
    void renderZombies();
    //加载植物弹幕
    void renderPlantBullet();
    //加载游戏内的各种UI
    void renderLevelGUI();
    //生成阳光
    void generateSunshine();
    //更新阳光的状态
    void updateSunshineState();
    //加载阳光
    void renderSunshine();

    //处理时间
    void handleEvents();
    //键盘按下
    void handleKeyDownEvent();
    //键盘松开
    void handleKeyUpEvent();
    //鼠标按下
    void handleMouseDownEvent();
    void handleMouseButtonRight();
    void handleMouseButtonLeft();
    void handleMouseButtonMiddle();
    //鼠标松开
    void handleMouseUpEvent();
    //鼠标移动
    void handleMouseMotionEvent();

    void highlightPlantTarget();
    
        
};

class PlayerAction{
public:
    void setPlant(plantTypeInGrid plantType,int row,int col);
};





#endif