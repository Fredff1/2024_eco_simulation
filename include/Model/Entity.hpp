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

struct EntityAgeFeature;

enum EntityDirection{
    ENTITY_NORTH_DIR,
    ENTITY_NORTHEAST_DIR,
    ENTITY_EAST_DIR,
    ENTITY_SOUTHEAST_DIR,
    ENTITY_SOUTH_DIR,
    ENTITY_SOUTHWEST_DIR,
    ENTITY_WEST_DIR,
    ENTITY_NORTHWEST_DIR,
};

enum EntityMovingState{
    ENTITY_STABLE,
    ENTITY_MOVING,
    ENTITY_ROTATING,

};

struct EntityMovingFeature{
    EntityDirection currentDirection=ENTITY_NORTH_DIR;
    EntityMovingState movingState=ENTITY_MOVING;
    int movingAngle=0;

    int convertDirToAngle(){
        return (45*static_cast<int>(currentDirection));
    }

    void addMovingAngle(int ang){
        movingAngle+=ang;
        if(movingAngle>=360){
            movingAngle=0;
        }else if(movingAngle<0){
            movingAngle=0;
        }
    }
};



struct EntityHungerFeature{
    int currentHuger;
    int maxHunger;
    bool isHungerAvailable=false;
    void addHunger(int hunger){
        currentHuger+=hunger;
        if(currentHuger>maxHunger){
            currentHuger=maxHunger;
        }
    }
    EntityHungerFeature(Gene& gene,bool flag,float shrinkRate){
        isHungerAvailable=flag;
        maxHunger=gene.calculateActualMaxHunger()/shrinkRate;
        maxHunger/=shrinkRate;
        currentHuger=maxHunger;
    }

    bool ifHungerLessThanHalf(){
        return currentHuger<maxHunger/2;
    }

    bool isFull(){
        return currentHuger>=maxHunger;
    }



    void setFull(){
        currentHuger=maxHunger;
    }
};

struct EntityAgeFeature{
    bool isAgeAvailable=true;
    int currentAge;
    int peakAge;
    int maxAge;
    int agePhase=0;
    int updateCooldown=0;
    int updateMaxCooldown=10;
    void addAge(int ageI){
        currentAge+=ageI;
    }
    EntityAgeFeature(Gene& gene,bool flag,float shrinkRate){
        isAgeAvailable=flag;
        currentAge=0;
        maxAge=gene.calculateActualMaxAge()/shrinkRate;
        peakAge=maxAge/2;
    }
};

struct EntityHealthFeature{
    int currentHealth;
    int geneMaxHealth;
    int currentMaxHealth;
    void addCurrentHealth(int health){
        currentHealth+=health;
        if(currentHealth>currentMaxHealth){
            currentHealth=currentMaxHealth;
        }
    }
    void addCurrentHealth(float rate){
        currentHealth+=static_cast<int>(rate*currentMaxHealth);
    }
    void changeMaxHealth(EntityAgeFeature& ageFeature){
        if(ageFeature.currentAge<=ageFeature.peakAge){
            currentMaxHealth=0.5*geneMaxHealth * (float(ageFeature.currentAge) / ageFeature.peakAge)+0.5*geneMaxHealth;
        }else{
            float ageDecline = float(ageFeature.currentAge - ageFeature.peakAge)*0.9 / (ageFeature.maxAge - ageFeature.peakAge);
        }
    }

    EntityHealthFeature(Gene& gene,EntityAgeFeature& ageFe,float shrinkRate){
        geneMaxHealth=gene.calculateActualMaxHealth()/shrinkRate;
        changeMaxHealth(ageFe);
        currentHealth=currentMaxHealth;
    } 
};

struct EntityReproductionFeature{
    int reproductionCount=0;
    int maxReproductionCount;

    EntityReproductionFeature(Gene& gene,float shrinkRate){
        maxReproductionCount=1500/gene.calculateReproductionRate()/shrinkRate;
    }

    void addRate(int count){
        reproductionCount+=count;
    }

    /*Set reproduction count 0*/
    void initRate(){
        reproductionCount=0;
    }

    bool getIfReproReady(){
        return reproductionCount>maxReproductionCount;
    }
};

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

struct EntityFeatureInitMsg{
    bool isHungerAvailable;
    bool isAgeAvailable;
    float shrinkRate;

    EntityFeatureInitMsg(bool hungerFLag,bool ageFlag,float shrink_rate):isHungerAvailable(hungerFLag),
    isAgeAvailable(ageFlag),shrinkRate(shrink_rate){

    }
};


struct EntityFeature{
    int id;
    EntityType type;
    Gene gene;
    EntityAgeFeature ageFeature;
    EntityHealthFeature healthFeature;
    EntityHungerFeature hungerFeature;
    EntityReproductionFeature reproductionFeature;
    
    EntityMovingFeature movingFeature;
    bool isHunting=false;
    SDL_Rect rectInAtlas;
    SDL_Color color={128,128,128,128};
    int maxSize;

    bool isAlive=true;

    
    EntityFeature(EntityFeatureInitMsg msg):gene(),ageFeature(gene,msg.isAgeAvailable,msg.shrinkRate),
    healthFeature(gene,ageFeature,msg.shrinkRate),hungerFeature(gene,msg.isHungerAvailable,msg.shrinkRate),
    reproductionFeature(gene,msg.shrinkRate){
        update();
    }

    EntityFeature():gene(),ageFeature(gene,true,1),
    healthFeature(gene,ageFeature,1),hungerFeature(gene,true,1),
    reproductionFeature(gene,1){
        update();
    }

    void update(){
        updateAge();
        updateHunger();
        updateAliveState();
        calculateSize(ageFeature.currentAge);
    }

    void updateAliveState(){
        if(healthFeature.currentHealth<0){
            isAlive=false;
        }
    }

    void updateColor(){
        color.g=10;
        color.r=10;
        color.b=10;
        if(type==CONSUMER_TYPE){
            color.r=gene.calculateMutateRate()*30+70;
        }else if(type==PRODUCER_TYPE){
            color.g=gene.calculateMutateRate()*30+70;
        }
        color.a=240;
    }

    void updateHunger(){
        if(!hungerFeature.isHungerAvailable){
            return;
        }
        if(hungerFeature.currentHuger<0){
            healthFeature.addCurrentHealth((float)-0.008);
        }
    }

    

    void calculateSize(int age){
        float rate=0.3+0.7*((float)ageFeature.currentAge/ageFeature.maxAge);
        rectInAtlas.h=maxSize*rate;
        rectInAtlas.w=maxSize*rate;
        
    }

    void updateAge(){
        if(ageFeature.currentAge>ageFeature.maxAge){
            isAlive=false;
        }else{
            if(ageFeature.updateCooldown<ageFeature.updateMaxCooldown){
                ageFeature.updateCooldown++;
            }else{
                ageFeature.updateCooldown=0;
                healthFeature.changeMaxHealth(this->ageFeature);
            }
        }
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
    
    EntityDirection convertDirectionIndex(int index){
        switch(index){
            case 0:
            return ENTITY_SOUTH_DIR;
            case 1:
            return ENTITY_NORTH_DIR;
            case 2:
            return ENTITY_EAST_DIR;
            case 3:
            return ENTITY_WEST_DIR;
            case 4:
            return ENTITY_SOUTHEAST_DIR;
            case 5:
            return ENTITY_NORTHWEST_DIR;
            case 6:
            return ENTITY_NORTHEAST_DIR;
            case 7:
            return ENTITY_SOUTHWEST_DIR;
            default:
            return ENTITY_SOUTH_DIR;
        }
    }
   
public:
    virtual ~Entity(){}
    Entity(int id,EntityType type,Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode,EntityFeatureInitMsg msg);

    Entity(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode);

    void setCurrentNode(std::shared_ptr<QuadTreeAtlasNode> targetNode);

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
