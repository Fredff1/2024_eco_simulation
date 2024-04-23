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
    void updateEntityState(){
        //state.color;
        //state.position.x=rectInAtlas.x,state.position.y=rectInAtlas.y;
        state.rectInAtlas=feature.rectInAtlas;
    }
public:
    ~Producer(){}

    Producer(int id, EntityType type, Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,type,position,currentNode){
       feature.rectInAtlas.x=position.x,feature.rectInAtlas.y=position.y;
       this->id=id;
       feature.maxSize=12;
       feature.update();
       feature.changeMaxDate(5);
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

    void checkWaterFlow(const std::list<std::shared_ptr<WaterFlow>>& flowList){
        for(auto flow:flowList){
            if(intersect_(feature.rectInAtlas,flow->targetArea)){
                auto vec=flow->calculateFlowAt(feature.rectInAtlas.x,feature.rectInAtlas.y);
                nextMotion.first=vec.x;
                nextMotion.second=vec.y;
                return;
            }
        }
        nextMotion.first=0;
        nextMotion.second=0;
        
        
    }

    void checkEnvironment();

    
       
    

   

    std::weak_ptr<QuadTreeAtlasNode> getCurrentNode() override;

};













#endif