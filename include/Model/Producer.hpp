#ifndef PRODUCER_H
#define PRODUCER_H

#include "../Model/Entity.hpp"
#include "../Util/GlobalStruct.hpp"
#include "Util/GlobalFunc.hpp"
#include "Model/WaterFlow.hpp"
#include <SDL2/SDL.h>

#include <utility>

class Producer:public Entity{
private:
    std::pair<int,int> nextMotion;
    int moveInterval=5;
    int moveCount=0;
    EntityMoveChoice nextMoveCHoice=NO_ENTITY_MOVE;

    

public:
    ~Producer(){}

    Producer(int id, EntityType type, Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,type,position,currentNode,
    EntityFeatureInitMsg(true,true,2.5)){
       feature.rectInAtlas.x=position.x,feature.rectInAtlas.y=position.y;
       feature.maxSize=20;
       initProducerFeature();
       feature.hungerFeature.setFull();
       /* color is fixed when initializing*/
       feature.updateColor();
    }

    Producer(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,feature,currentNode){
        feature.id=id;
        feature.hungerFeature.setFull();
        initProducerFeature();
        feature.updateColor();
    }

    void initProducerFeature(){
        feature.type=PRODUCER_TYPE;
        feature.update();
        updateEntityState();
    }

    void updateEntityState(){
        //state.color;
        //state.position.x=rectInAtlas.x,state.position.y=rectInAtlas.y;
      
        // if(state.rectInAtlas.h>50){
        //     std::cout<<"Error for Producer "<<"id:"<<feature.id<<std::endl;
        // }
    }

   

   

   
    
    

    
    /* render it self for GUI*/
    void render(SDL_Renderer* renderer,const SDL_Rect& rectInUI)override{
        SDL_SetRenderDrawColor(renderer,0,200,0,255);
        SDL_RenderFillRect(renderer,&rectInUI);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
    }

    void update(QuadTreeAtlas& quadTreeAtlas) override;
    

    float calculateLifeLoss(float fitness);
    

    
    void actReproduction(QuadTreeAtlas& quadTreeAtlas);
    

    void move();

    void checkWaterFlow();
        
        
        
    

    void checkEnvironment();

    void updateHunger();
       
    

   

    std::weak_ptr<QuadTreeAtlasNode> getCurrentNode() override;

};













#endif