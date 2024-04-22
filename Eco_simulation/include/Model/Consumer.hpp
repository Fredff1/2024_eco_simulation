#ifndef CONSUMER_H
#define CONSUMER_H
#include "../Model/Entity.hpp"
#include "../Util/GlobalStruct.hpp"
#include <SDL2/SDL.h>


class Consumer :public Entity{
private:
    EntityMoveChoice nextMoveChoice=NO_ENTITY_MOVE;
    
    bool randomMove=true;
    void updateEntityState(){
        state.rectInAtlas=feature.rectInAtlas;
    }
    
public: 
    ~Consumer(){}

    Consumer(int id, EntityType type, Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):Entity(id,type,position,currentNode){
        this->id=id;
        feature.rectInAtlas.x=position.x,feature.rectInAtlas.y=position.y;
        feature.maxSize=35;
        feature.update();
        feature.isHungerAvailable=true;
        feature.maxReproductionCount/=3;
    }

   

    void render(SDL_Renderer* renderer,const SDL_Rect& rectInUI) override{
        SDL_SetRenderDrawColor(renderer,0,200,0,255);
        SDL_RenderFillRect(renderer,&rectInUI);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
    }

    

    void update(QuadTreeAtlas& quadTreeAtlas) override;
       
        
    

    void setMoveChoice();

    void move();

    std::weak_ptr<QuadTreeAtlasNode> getCurrentNode();

    void checkHunger(QuadTreeAtlas& quadTreeAtlas);

    void tryHunt(QuadTreeAtlas& quadTreeAtlas);

    void actReproduction(QuadTreeAtlas& quadTreeAtlas);

    int computeDirection();
    
};









#endif