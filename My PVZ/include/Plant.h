
#ifndef PLANT_H
#define PLANT_H
//#include "Zombie.h"
#include "GUIRes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameGrid;
class Level;

class Sunshine{

public:
    Sunshine(int xI, int yI, int valueI,SDL_factory& GUI_factory,SunshineState stateI=PLANT_GENERATED,SunshineType typeI=NORMALSUNSHINE); // 构造函数
    SDL_Rect destRect;
    SDL_Point gathered_dest;
    SDL_Point falling_dest;
    SDL_Point up_dest;
    bool isDirectionChange;
    int acceleration_y;
    std::map<std::string,SDL_Texture*> textures;
    SunshineType type;
    SunshineState state;
    //移动到阳关栏的速度
    int x_off,y_off;
    int x,y;
    int value;
    int disappearCount,disappearMaxTime;
    bool is_collected;
    void update(); // 更新阳光状态，例如下落动画
    void render(SDL_Renderer* renderer); // 绘制阳光到屏幕上
    bool isCollected(int mouseX,int mouseY); // 检查阳光是否被收集
    
};


class Plant{
public:
    enum plantCategory{
        PeaFamily=1,
        MushroomFamily=2,
        Water_family=3,
    };
    std::map<std::string,SDL_Texture*> texture; // 添加一个用于渲染的纹理
    std::map<std::string,SDL_Rect> srcRects; // 纹理的源矩形
    std::map<std::string,SDL_Rect> destRects; // 渲染到屏幕上的目标矩形
    std::map<std::string,SDL_Rect> collisionRects;
    int zoom_rate;
    int rowPos;
    int colPos;
    //记录植物的位置是内层还是外层 正常为1 南瓜为2 咖啡豆等为0
    int grid_layer_pos;
    float grid_x_pos;//0-7200
    float grid_y_pos;//0-3000
    PlantFeature feature;
    bool is_attacking;
    bool can_be_placed;
    bool facing_right;
    Plant(int healthI,int costI){
        feature.health=healthI,feature.cost=costI,zoom_rate=5;
        is_attacking=false;
        feature.attackCoolDown=0;
        grid_layer_pos=1;
    }
    virtual void act(Level& playingLevel)=0; 
    virtual void renderPlant(SDL_Renderer* renderer,const BackGroundState state)=0;
    virtual void testEnemy(const vector<vector<GameGrid>>& grid)=0;
    virtual void init_plant(const int row,const int col,const BackGroundState state)=0;
    void loadPlantFeature(const plantTypeInGrid type);
protected:
    
};


class PlantBullet{
public:
    PlantBullet(float posX, float posY, float velX, float velY, int damageI){
        grid_x_pos=posX,grid_y_pos=posY,velocityX=velX,velocityY=velY,damage=damageI;
        isGridChanged=false;
        isActive=true;
        pierceAbility=CANNOT_PIERCE,MaxPierceNum=0,pierceCount=0;
    }
    void update(const BackGroundState state);
    void render(SDL_Renderer* renderer,const BackGroundState state);
    void updatePierceAbility();
    bool checkCollision(const SDL_Rect& targetRect);
    std::map<std::string,SDL_Texture*> textures;
    std::vector<int> attack_zombie_ids;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    SDL_Rect collisionRect;
    BulletPierce pierceAbility;
    int pierceCount,MaxPierceNum;
    int rowPos,colPos;
    float grid_x_pos, grid_y_pos;
    float velocityX, velocityY;
    bool isActive;
    bool isGridChanged;
    int damage;
    float rotation;
    SDL_RendererFlip flip;
    int currentFrame;
    int totalFrames;
    int frameDelay;
    Uint32 lastFrameUpdateTime;
    int direction;
private:
    

};

class PeaShooter:public Plant{
private:
    
public:
    PeaShooter(int health=300,int cost=100):Plant(health,cost){
        loadPlantFeature(PeaShooterType);
        can_be_placed=true,facing_right=true;
    }
    void renderPlant(SDL_Renderer* renderer,const BackGroundState state) override;
    void act(Level& playingLevel) override;
        //more features to add
    unique_ptr<PlantBullet> attack();   
    void testEnemy(const vector<vector<GameGrid>>& grid) override;
    void init_plant(const int row,const int col,const BackGroundState state) override;
    

};

class Sunflower:public Plant{
private:
    
public:
    int GenerateCount,GenerateMaxCount;
    SDL_factory GUI_factory;
    
    Sunflower(int health=300,int cost=50):Plant(health,cost){
        loadPlantFeature(SunflowerType);
        can_be_placed=true,facing_right=true;
        GenerateCount=1200,GenerateMaxCount=1300;
    }
    void renderPlant(SDL_Renderer* renderer,const BackGroundState state) override;
    void act(Level& playingLevel) override;
        //more features to add
    void testEnemy(const vector<vector<GameGrid>>& grid) override;
    void init_plant(const int row,const int col,const BackGroundState state) override;
    Sunshine generateSunshine();
    void act_add_sunshine(Level& playingLevel);
     

};



class PlantFactory{
private:
    SDL_factory _plant_res;
    int zoom_rate;
    enum texture_name{
        head=0,
        base=1,
    };
public:
    unique_ptr<Plant> createPlant(const std::string&name,const int row,const int col,const BackGroundState state);    
    
    PlantFactory(){
        zoom_rate=5;
    }
};

#endif