#ifndef CONSUMER_H
#define CONSUMER_H
#include "../Model/Entity.hpp"
#include "../Util/GlobalStruct.hpp"
#include <deque>
#include <SDL2/SDL.h>

struct LockedTargetEnt{
    bool isLocked=false;
    bool hasFind=false;
    int targetID;
    void lockOn(int id){
        hasFind=true;
        targetID=id;
        isLocked=true;
    }

    void deleteLock(){
        hasFind=false;
        isLocked=false;
        targetID=-1;
    }
};

class Consumer :public Entity{
private:
    EntityMoveChoice nextMoveChoice=NO_ENTITY_MOVE;
    bool randomMove=true;
    int moveCooldown=0;
    int maxMoveCooldown;
    
    std::deque<int> moveHistory;
    
    
    
public: 


    ~Consumer(){}
    LockedTargetEnt lockEntityState;
    Consumer(int id, EntityType type, Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,type,position,currentNode,
    EntityFeatureInitMsg(true,true,1)){
        this->feature.id=id;
        feature.rectInAtlas.x=position.x,feature.rectInAtlas.y=position.y;
        feature.maxSize=75;
        initConsumerFeature();
        feature.updateColor();
    }

    Consumer(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,feature,currentNode){
        this->feature.id=id;
        initConsumerFeature();
        feature.updateColor();
    }

    void initConsumerFeature(){
        feature.type=CONSUMER_TYPE;
        feature.update();
        moveCooldown=0,maxMoveCooldown=0;
    }

    void render(SDL_Renderer* renderer,const SDL_Rect& rectInUI) override{
        SDL_SetRenderDrawColor(renderer,0,200,0,255);
        SDL_RenderFillRect(renderer,&rectInUI);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
    }

    EntityDirection getCurrentDirection(){
        auto dir=moveHistory.back();
        return static_cast<EntityDirection>(dir);
    }

    void update(QuadTreeAtlas& quadTreeAtlas) override;
       
    void updateFeature(){
        feature.updateAge();
        feature.updateAliveState();
        //feature.updateHunger();
        // feature.currentHuger-=2;
        // if(feature.currentHuger<=0){
        //     feature.currentHuger-=feature.currentMaxHealth*0.002;
        // }else{
        //     feature.currentHealth+=feature.currentMaxHealth*0.0005;
        // }
    }
    

    void setMoveChoice();

    void move();

    std::weak_ptr<QuadTreeAtlasNode> getCurrentNode();

    void checkHunger(QuadTreeAtlas& quadTreeAtlas);

    void tryHunt(QuadTreeAtlas& quadTreeAtlas);

    void actReproduction(QuadTreeAtlas& quadTreeAtlas);

    int computeDirection();

    float calculateLifeLoss(float fitness);

    void checkEnvironment();

    int determineBestDirection(std::vector<float>& currentDirectionScores);

    void updateMoveHistory(int bestDirection);
    
};









#endif