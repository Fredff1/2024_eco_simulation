#include "Producer.hpp"
#include <SDL2/SDL.h>
#include "Model/QuadTreeAtlas.hpp"
#include "Util/RandomUtil.hpp"

std::weak_ptr<QuadTreeAtlasNode> Producer::getCurrentNode(){
    return currentNode;
}

void Producer::move(){
        //生成一个0到7之间的随机索引
        int index =RandomUtil::getRandomInt(0,7);

        //index=4;
        // 获取随机方向
        auto [dx, dy] = directions[index];

        // 更新位置
        checkWaterFlow();
        
        feature.rectInAtlas.x = (feature.rectInAtlas.x + nextMotion.first * 2 + 4096) % 4096;  // 使用取模操作实现环绕
        feature.rectInAtlas.y = (feature.rectInAtlas.y + nextMotion.second * 2 + 4096) % 4096;
        
        
}

void Producer::updateHunger(){
    auto node=currentNode.lock();
    int lightIn=feature.rectInAtlas.h*this->feature.rectInAtlas.w*4;
    feature.hungerFeature.addHunger(-lightIn/4);
    if(node->resource.getLightResource()>lightIn){
        node->resource.minusLightResource(lightIn);
        feature.hungerFeature.addHunger(lightIn/4);
    }else{
        feature.hungerFeature.addHunger(node->resource.getLightResource());
        node->resource.setLightResource(0);
    }
}

void Producer::update(QuadTreeAtlas& quadTreeAtlas) {
    checkEnvironment();
    updateHunger();
    feature.healthFeature.addCurrentHealth(feature.rectInAtlas.h);
    if(moveCount>=moveInterval){
        this->move();
        moveCount=0;
    }else{
        moveCount++;
    }

    
    feature.update();
    actReproduction(quadTreeAtlas);
    updateEntityState();
   
    
}

/* Return <0 if not fit*/
float Producer::calculateLifeLoss(float fitness) {

    // 设定最大生命值损失比例
    const float maxLossPercentage = 0.0008;  // 最大损失为最大生命值的0.1%
    float maxLoss = feature.healthFeature.currentMaxHealth * maxLossPercentage;
    if(fitness>0.6){
        return (0.1)*maxLoss;
    }else if(fitness>0.4){
        return 0;
    }else if(fitness>0){
        return -(0.6-fitness)*maxLoss;
    }else{
        return maxLoss;
    }
    
}

void Producer::checkEnvironment(){
    auto node=currentNode.lock();
    int pos_x=feature.rectInAtlas.x;
    int pos_y=feature.rectInAtlas.y;
    auto act_ocy=node->environmentFactor.computeOxygenInAtlas(pos_x,pos_y,DEFAULT_ATLAS_STATE);
    auto act_tem=node->environmentFactor.computeTempInAtlas(pos_x,pos_y,DEFAULT_ATLAS_STATE);
    auto act_ph=node->environmentFactor.computePHInAtlas(pos_x,pos_y,DEFAULT_ATLAS_STATE);
    auto oxy_fac=feature.gene.calculateActualOxygen(act_ocy);
    auto tem_fac=feature.gene.calculateActualTemperature(act_tem);
    auto ph_fac=feature.gene.calculateActualPH(act_ph);
    feature.healthFeature.addCurrentHealth(calculateLifeLoss(oxy_fac)+calculateLifeLoss(tem_fac)+calculateLifeLoss(ph_fac));
}

void Producer::actReproduction(QuadTreeAtlas& quadTreeAtlas){
    if(feature.reproductionFeature.getIfReproReady()==false){
       
        feature.reproductionFeature.addRate(1);
        
        
    }else{
        if(currentNode.lock()->entities.size()>MAX_ENTITIES){
            return;
        }
        
        auto newRect=feature.rectInAtlas;
        newRect.x=(newRect.x+4096+RandomUtil::getRandomInt(-240,240))%4096;
        newRect.y=(newRect.y+4096+RandomUtil::getRandomInt(-240,240))%4096;
        auto ent=quadTreeAtlas.getEntityFactory().createEntity(PRODUCER_TYPE,newRect ,quadTreeAtlas.getRoot());
        this->feature.gene.copyGeneTo(ent->getGene());
        ent->getFeature().updateColor();
        quadTreeAtlas.getEntityFactory().entityToAdd.push_back(std::move(ent));
        feature.reproductionFeature.initRate();
        //std::cout<<"Create a producer"<<std::endl;
    }
}

void Producer::checkWaterFlow(){
    nextMotion.first=0;
    nextMotion.second=0;
    auto node=currentNode.lock();
    for(auto flow:node->waterFlowList){
        bool flag_1=intersect_(feature.rectInAtlas,flow->feature.targetArea);
        if(flag_1){
            auto vec=flow->calculateFlowAt(feature.rectInAtlas.x,feature.rectInAtlas.y);
            nextMotion.first+=vec.x;
            nextMotion.second+=vec.y;
        }else{
            int a=0;
        }
    }
}
