#ifndef ENTITY_H
#define ENTITY_H

#include "GlobalEnum.hpp"
#include "GlobalStruct.hpp"
#include "Util/Observers.hpp"
#include "Gene.hpp"
#include <SDL2/SDL.h>
#include <random>
#include <memory>
#include <chrono>

struct QuadTreeAtlasNode;

class QuadTreeAtlas;

class EntityState{
public:
    int id;
    Point position;
    EntityType type;
    SDL_Rect rectInAtlas;
    SDL_Color color;
    bool isAlive;
    
    
    //more to add

    EntityState(int id_,Point position_,EntityType type_):id(id){
        type=type_;
        position=position_;
        
    }

};


struct EntityFeature{
    int id;
    EntityType type;

    int currentHealth;
    int geneMaxHealth;
    int currentMaxHealth;

    int currentHuger;
    int maxHunger;
    bool isHungerAvailable=false;
    bool isHunting=false;

    int currentAge;
    int peakAge;
    int maxAge;
    int agePhase;

    int reproductionCount=0;
    int maxReproductionCount;

    Gene gene;

    SDL_Rect rectInAtlas;
    SDL_Color color={128,128,128,128};
    int maxSize;

    bool isAlive=true;

    void update(){
        updateAge();
        updateHunger();
        if(currentHealth<=0){
            isAlive=false;
        }

    }

    void updateColor(){
        color.g=10;
        if(type==CONSUMER_TYPE){
            color.r=gene.calculateMutateRate()*2/3+70;
            color.b=10;
        }else if(type==PRODUCER_TYPE){
            color.g=gene.calculateMutateRate()*2/3+70;
            color.b=10;
        }
        color.a=gene.calculateMutateRate();
    }

    void updateHunger(){
        if(!isHungerAvailable){
            return;
        }
        currentHuger-=3;
        if(currentHuger<0){
            currentHealth-=currentMaxHealth*0.005;
        }
    }

    float calculateHealth(int age) {
        if (age <= peakAge) {
            return 0.5*geneMaxHealth * (float(age) / peakAge)+0.5*geneMaxHealth;  // 线性增长至最大健康
        } else {
            // 假设健康值从峰值线性下降至0
            float ageDecline = float(age - peakAge)*0.9 / (maxAge - peakAge);
            return geneMaxHealth * (1 - ageDecline);
        }
    }

    void calculateSize(int age){
        float rate=0.2+0.8*((float)currentAge/maxAge);
        rectInAtlas.h=maxSize*rate;
        rectInAtlas.w=maxSize*rate;
        
    }

    void updateAge(){
        currentAge++;
        if(currentAge>maxAge){
            isAlive=false;
        }else{
            currentMaxHealth=calculateHealth(currentAge);
            calculateSize(currentAge);

        }
    }

    /* shrink for producer*/
    void changeMaxDate(int rate){
        maxAge/=rate;
        peakAge=maxAge/2;
        maxHunger/=rate;
        currentHuger=maxHunger;
        geneMaxHealth/=rate;
        currentMaxHealth=calculateHealth(currentAge);
        currentHealth=currentMaxHealth;
        maxReproductionCount=maxAge/gene.calculateReproductionRate();
    }

    EntityFeature(){
        maxAge=gene.calculateActualMaxAge();
        peakAge=maxAge/2;
        currentAge=0;

        maxHunger=gene.calculateActualMaxHunger();
        currentHuger=maxHunger;

        geneMaxHealth=gene.calculateActualMaxHealth();
        currentMaxHealth=calculateHealth(currentAge);
        currentHealth=currentMaxHealth;

        maxReproductionCount=maxAge/gene.calculateReproductionRate();
        updateColor();
        update();

    }

    void readPosData(int x,int y,int mSize){
        rectInAtlas.x=x,rectInAtlas.y=y;
        maxSize=mSize;
        update();
    }
};

class Entity{ 
protected:
    
    //Position in the atlas,not in the window
    
    
    /* Health of the entity*/
    EntityFeature feature;
    
    



    std::weak_ptr<QuadTreeAtlasNode> currentNode;
    std::pair<int, int> directions[8] = {
            {0, 1},   // 下
            {0, -1},  // 上
            {1, 0},   // 右
            {-1, 0},  // 左
            {1, 1},   // 右下
            {-1, -1}, // 左上
            {1, -1},  // 右上
            {-1, 1}   // 左下
        };
   
public:
    virtual ~Entity(){}
    Entity(int id,EntityType type,Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode);

    Entity(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode);

    void setCurrentNode(std::shared_ptr<QuadTreeAtlasNode>& targetNode);

    //update the entity state
    virtual void update(QuadTreeAtlas& quadTreeAtlas) =0;

    Point getPos() {
        return Point(feature.rectInAtlas.x,feature.rectInAtlas.y);
    }

    EntityFeature& getFeature(){
        return this->feature;
    }

    EntityType getEntityType() const{
        return this->feature.type;
    }

    /* get the id of the entity*/
    int getID() const{
        return this->feature.id;
    }
    //get the current state to pass to the GUI thread
    // EntityState getEntityState() const{
    //     return this->state;
    // }


    SDL_Rect getRectInAtlas() const{
        return feature.rectInAtlas;
    }


    virtual void render(SDL_Renderer* renderer,const SDL_Rect& rectInUI)=0;
    
    void setPos(float x,float y){
       
    }

    void setRectInAtlas(SDL_Rect rect){
        feature.rectInAtlas=rect;
    }
 
    virtual std::weak_ptr<QuadTreeAtlasNode> getCurrentNode()=0;

    bool getAlive(){
        return feature.isAlive;
    }

    Gene& getGene(){
        return this->feature.gene;
    }

};






















#endif
