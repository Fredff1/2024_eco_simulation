#include "Consumer.hpp"
#include <SDL2/SDL.h>
#include "Model/QuadTreeAtlas.hpp"

std::weak_ptr<QuadTreeAtlasNode> Consumer::getCurrentNode(){
    return this->currentNode;
}

void Consumer::move(){
        if(feature.currentHuger>=(feature.maxHunger*9/10)){
            return;
        }
        int index=computeDirection();
        if(randomMove){
            // 生成一个0到7之间的随机索引
            //int index =RandomUtil::getRandomInt(0,7);
            // int index;
            // index=RandomUtil::getRandomInt(0,7);
            
        }
        
        //index=4;
        // 获取随机方向
        
        auto [dx, dy] = this->directions[index];
        feature.rectInAtlas.x = (feature.rectInAtlas.x + dx * 2 + 4096) % 4096;  // 使用取模操作实现环绕
        feature.rectInAtlas.y = (feature.rectInAtlas.y + dy * 2 + 4096) % 4096;
        // 更新位置
        
        //randomMove=true;
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
        if(feature.currentHuger<feature.maxHunger/4||feature.currentHealth<feature.currentMaxHealth/3){
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

int Consumer::computeDirection(){
        auto currentRect=feature.rectInAtlas;
        auto searchRect=currentRect;
        searchRect.x-=3*currentRect.w;
        searchRect.y-=3*currentRect.h;
        searchRect.h*=4;
        searchRect.w*=4;
        auto tree=currentNode.lock()->quadTree;
        //上 下 左 右
        int directionScore[8]={0,0,0,0,0,0,0,0};
        tree.applyToNode(tree.getRoot(),searchRect,[&directionScore,currentRect,this](std::shared_ptr<QuadTreeAtlasNode>& node){
            for(auto ent:node->entities){
                int type_score;
                switch(ent->getEntityType()){
                    case PRODUCER_TYPE:
                    if(this->getFeature().currentHuger<=this->getFeature().maxHunger/2){
                        type_score=3;
                    }else{
                        type_score=1;
                    }
                    break;
                    case CONSUMER_TYPE:
                    //if(this->getFeature().currentHuger>=this->getFeature().maxHunger/2){
                    type_score=-2;
                    //}
                    //type_score=0;
                    
                    break;
                }
                auto entRect=ent->getFeature().rectInAtlas;
                // 水平方向
                if(entRect.x < currentRect.x){
                    directionScore[3]+=type_score;  // 左
                } else {
                    directionScore[2]+=type_score;  // 右
                }
                // 垂直方向
                if(entRect.y < currentRect.y){
                    directionScore[1]+=type_score;  // 上
                } else {
                    directionScore[0]+=type_score;  // 下
                }
                // 对角方向
                if(entRect.x < currentRect.x && entRect.y < currentRect.y){
                    directionScore[5]+=type_score;  // 左上
                } else if(entRect.x > currentRect.x && entRect.y < currentRect.y){
                    directionScore[6]+=type_score;  // 右上
                } else if(entRect.x > currentRect.x && entRect.y > currentRect.y){
                    directionScore[4]+=type_score;  // 右下
                } else if(entRect.x < currentRect.x && entRect.y > currentRect.y){
                    directionScore[7]+=type_score;  // 左下
                }
            }
        });
        int bestDirection = 0;
        int maxScore = directionScore[0];
        for (int i = 1; i < 8; i++) {
            if (directionScore[i] > maxScore) {
                maxScore = directionScore[i];
                bestDirection = i;
            }
        }
        //randomMove=false;
        return bestDirection;
        
        
        
    }

