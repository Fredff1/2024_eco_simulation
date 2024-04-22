#include "Consumer.hpp"
#include <SDL2/SDL.h>
#include "Model/QuadTreeAtlas.hpp"

std::weak_ptr<QuadTreeAtlasNode> Consumer::getCurrentNode(){
    return this->currentNode;
}

void Consumer::move(){
    

        // 生成一个0到7之间的随机索引
        //int index =RandomUtil::getRandomInt(0,7);
        int index;
        index=RandomUtil::getRandomInt(0,7);
        //index=4;
        // 获取随机方向
        auto [dx, dy] = this->directions[index];

        // 更新位置
        feature.rectInAtlas.x = (feature.rectInAtlas.x + dx * 2 + 4096) % 4096;  // 使用取模操作实现环绕
        feature.rectInAtlas.y = (feature.rectInAtlas.y + dy * 2 + 4096) % 4096;
}

void Consumer::setMoveChoice(){

}

void Consumer::checkHunger(QuadTreeAtlas& quadTreeAtlas){
    if(feature.currentHuger<feature.maxHunger*2/3){
        feature.isHunting=true;
        tryHunt(quadTreeAtlas);
    }
}

void Consumer::tryHunt(QuadTreeAtlas& quadTreeAtlas){
    auto searchSize=feature.rectInAtlas.h/2;
    auto searchRect=feature.rectInAtlas;
    searchRect.x-=searchSize,searchRect.y-=searchSize;
    searchRect.h+=searchSize,searchRect.w+=searchSize;
    quadTreeAtlas.applyToNode(quadTreeAtlas.getRoot(),searchRect,[searchRect,this](std::shared_ptr<QuadTreeAtlasNode> & node){
            for(auto& entity:node->entities){
                if(this->getFeature().isHunting&&intersect_(searchRect,entity->getRectInAtlas())&&entity->getEntityType()==PRODUCER_TYPE){
                    auto& feature=entity->getFeature();
                    feature.isAlive=false;

                    this->getFeature().currentHuger+=(feature.rectInAtlas.h*feature.rectInAtlas.w*25);
                    int hunger_overflow=getFeature().currentHuger-getFeature().maxHunger;
                    if(hunger_overflow>0){
                        getFeature().currentHuger=getFeature().maxHunger;
                        getFeature().currentHealth+=hunger_overflow;
                        this->getFeature().isHunting=false;
                        return;
                    }
                }
            }
    });

}

void Consumer::update(QuadTreeAtlas& quadTreeAtlas){
    checkHunger(quadTreeAtlas);
    actReproduction(quadTreeAtlas);
    feature.currentHealth++;
    this->move();
    feature.update();
    this->updateEntityState();
}



void Consumer::actReproduction(QuadTreeAtlas& quadTreeAtlas){
    if(feature.reproductionCount<feature.maxReproductionCount){
        feature.reproductionCount++;
    }else{
        // if(currentNode.lock()->entities.size()>MAX_ENTITIES){
        //     return;
        // }
        if(feature.currentHuger<feature.maxHunger/2||feature.currentHealth<feature.currentMaxHealth/3){
            return;
        }
        auto newRect=feature.rectInAtlas;
        newRect.x=(newRect.x+4096+RandomUtil::getRandomInt(-40,40))%4096;
        newRect.y=(newRect.y+4096+RandomUtil::getRandomInt(-40,40))%4096;
        auto ent=quadTreeAtlas.getEntityFactory().createEntity(CONSUMER_TYPE,newRect ,quadTreeAtlas.getRoot());
        quadTreeAtlas.entityToAdd.push_back(ent);
        feature.reproductionCount=0;
        std::cout<<"Create a consumer"<<std::endl;
    }
}