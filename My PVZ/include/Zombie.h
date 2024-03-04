
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "GUIRes.h"
#include "plant.h"
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using std::unique_ptr;



class ZombieProtective{
public:    
    SDL_Rect DestRect;
    SDL_Rect CollisionRect;
    SDL_Texture* texture;
    int health;
    bool is_pierce_available;
};


class Zombie{
private:
    
public:
    static int nextId; // 用于生成唯一ID的静态成员变量
    int id; // 每个僵尸的唯一ID
    std::map<std::string,SDL_Texture*> texture; // 添加一个用于渲染的纹理
    SDL_Rect srcRect; // 纹理的源矩形
    SDL_Rect destRect; // 渲染到屏幕上的目标矩形
    std::map<std::string,SDL_Rect> collisionRectMap;//碰撞箱
    
    float jumpHeight; // 跳动的振幅，即僵尸跳动的最大高度
    float jumpFrequency; // 跳动的频率，影响跳动的快慢
    float baseYPos;
    ZombieState zombieState;
    ZombieFeature feature;
    std::vector<ZombieProtective> protectiveList;

    std::vector<ZombieArmor> armors;
    int rowPos;
    int colPos;
    float grid_x_pos;//0-7200
    float grid_y_pos;//0-3000
    int zoom_rate;

    float angle;
    float angle_change;
    bool isGridChanged;
    bool isDying;
    bool isDead;
    bool facing_left;
    Zombie(int health_input=100,float speed_input=4,int zombieTypeInput=NoZombieType):grid_x_pos(GRID_LENGTH),colPos(0),isGridChanged(false){
        facing_left=true,isDying=false,isDead=false,zombieState=MOVING,angle=0.0;
        angle_change=0,id=nextId++;
    }
    virtual void move()=0;
    virtual void attack(std::unique_ptr<Plant>& target)=0;
    virtual void render_zombie(SDL_Renderer* renderer,const BackGroundState state)=0;
    virtual void updateCollisionBox()=0;
    virtual void updateLivingCondition()=0;
    virtual void init_zombie(const int row)=0;
    virtual bool is_collied_with_plant(const std::map<std::string,SDL_Rect>& destCollisionRect)=0;
    virtual void act()=0;
    void loadZombieFeature(const ZombieType type);
protected:
    
};



class BasicZombie:public Zombie{
private:
public:
    BasicZombie(int health=270,float speed=4.0+rand()%3,ZombieType type_input=BasicZombieType):Zombie(health,speed,type_input){
        // 为跳动效果的参数设置一些随机的初始值
        jumpHeight = 10.0f + static_cast<float>(rand() % 20); // 在5到10之间随机选择振幅
        jumpFrequency = 10.0f + static_cast<float>(rand() % 5) / 1.0f; // 在0.1到0.2之间随机选择频率
        SDL_Rect BodyRect,HeadRect;
        collisionRectMap["head"]=HeadRect;
        collisionRectMap["body"]=BodyRect;
        zoom_rate=7;
        loadZombieFeature(BasicZombieType);
    }
    //每个游戏刻行走的距离

    void move() override;
    void attack(std::unique_ptr<Plant>& target) override;
    void render_zombie(SDL_Renderer* renderer,const BackGroundState state) override;
    void updateCollisionBox() override;
    void updateLivingCondition() override;
    void init_zombie(const int row)override;
    bool is_collied_with_plant(const std::map<std::string,SDL_Rect>& destCollisionRect);
    void act() override;
    
};  

class ConeHeadZombie:public Zombie{
public:    
    ConeHeadZombie(int health=270,float speed=4.0+rand()%3,ZombieType type_input=BasicZombieType);
      
    //每个游戏刻行走的距离

    void move() override;
    void attack(std::unique_ptr<Plant>& target) override;
    void render_zombie(SDL_Renderer* renderer,const BackGroundState state) override;
    void updateCollisionBox() override;
    void updateLivingCondition() override;
    void init_zombie(const int row)override;
    bool is_collied_with_plant(const std::map<std::string,SDL_Rect>& destCollisionRect);
    void act() override;
};

class ZombieFactory{
private:
    SDL_factory _zombie_res;
    enum texture_name{
        full_health=0,
    };
public:    
    unique_ptr<Zombie> createZombie(const std::string& name,const int row);
    void setZombiePos(unique_ptr<Zombie>& target_zombie,const int row);
    
};



#endif