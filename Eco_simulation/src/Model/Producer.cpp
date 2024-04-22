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

        index=RandomUtil::getRandomInt(0,7);
        //index=4;
        // 获取随机方向
        auto [dx, dy] = directions[index];

        // 更新位置
        feature.rectInAtlas.x = (feature.rectInAtlas.x + nextMotion.first * 2 + 4096) % 4096;  // 使用取模操作实现环绕
        feature.rectInAtlas.y = (feature.rectInAtlas.y + nextMotion.second * 2 + 4096) % 4096;
}

void Producer::updateHunger(){
    auto node=currentNode.lock();
    int lightIn=feature.rectInAtlas.h*this->feature.rectInAtlas.w;
    feature.currentHuger-=lightIn;
    if(node->resource.getLightResource()>lightIn){
        node->resource.minusLightResource(lightIn);
        feature.currentHuger+=lightIn;
    }else{
        feature.currentHuger+=node->resource.getLightResource();
        feature.currentHealth+=feature.currentHuger;
        node->resource.setLightResource(0);
        feature.currentHealth-=SDL_abs(feature.currentHuger);
    }
}

void Producer::update(QuadTreeAtlas& quadTreeAtlas) {
    checkEnvironment();
    updateHunger();
    checkWaterFlow(quadTreeAtlas.getWaterFlowList());
    if(moveCount>=moveInterval){
        this->move();
        moveCount=0;
    }else{
        moveCount++;
    }
    
    feature.update();
    actReproduction(quadTreeAtlas);
    this->updateEntityState();
}

/* Return <0 if not fit*/
float Producer::calculateLifeLoss(float fitness) {

    // 设定最大生命值损失比例
    const float maxLossPercentage = 0.05;  // 最大损失为最大生命值的10%
    float maxLoss = feature.currentMaxHealth * maxLossPercentage;

    // 生命值损失与适应度成反比
    
    if(fitness>0){
        float lifeAdd = maxLoss * (1 - fitness);
        return lifeAdd;
    }else{
        float lifeLoss=maxLoss*(1+fitness);
        return lifeLoss;
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
    feature.currentHealth=feature.currentHealth+calculateLifeLoss(oxy_fac)+calculateLifeLoss(tem_fac)+calculateLifeLoss(ph_fac);
    if(feature.currentHealth>feature.currentMaxHealth){
        feature.currentHealth=feature.currentMaxHealth;
    }
}

void Producer::actReproduction(QuadTreeAtlas& quadTreeAtlas){
    if(feature.reproductionCount<feature.maxReproductionCount){
        feature.reproductionCount++;
    }else{
        if(currentNode.lock()->entities.size()>MAX_ENTITIES){
            feature.reproductionCount--;
            return;
        }
        if(feature.currentHealth<feature.currentMaxHealth*2/3){
            return;
        }
        auto newRect=feature.rectInAtlas;
        newRect.x=(newRect.x+4096+RandomUtil::getRandomInt(-40,40))%4096;
        newRect.y=(newRect.y+4096+RandomUtil::getRandomInt(-40,40))%4096;
        auto ent=quadTreeAtlas.getEntityFactory().createEntity(PRODUCER_TYPE,newRect ,quadTreeAtlas.getRoot());
        quadTreeAtlas.entityToAdd.push_back(ent);
        feature.reproductionCount=0;
        std::cout<<"Create a producer"<<std::endl;
    }
}

