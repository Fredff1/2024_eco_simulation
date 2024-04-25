#include "Consumer.hpp"
#include <SDL2/SDL.h>
#include "Model/QuadTreeAtlas.hpp"

std::weak_ptr<QuadTreeAtlasNode> Consumer::getCurrentNode(){
    return this->currentNode;
}

void Consumer::move(){
        
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
        feature.rectInAtlas.x = (feature.rectInAtlas.x + dx * 3 + 4096) % 4096;  // 使用取模操作实现环绕
        feature.rectInAtlas.y = (feature.rectInAtlas.y + dy * 3 + 4096) % 4096;
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
    feature.currentHuger-=1;
}

void Consumer::tryHunt(QuadTreeAtlas& quadTreeAtlas){
    auto searchSize=feature.rectInAtlas.h*2;
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
                        //return;
                    }
                }
            }
    });

}

void Consumer::update(QuadTreeAtlas& quadTreeAtlas){
    checkHunger(quadTreeAtlas);
    checkEnvironment();
    actReproduction(quadTreeAtlas);
    feature.currentHealth+=feature.rectInAtlas.w*feature.rectInAtlas.h/5;
    this->move();
    feature.update();
    this->updateEntityState();
}

float Consumer::calculateLifeLoss(float fitness) {

    // 设定最大生命值损失比例
    const float maxLossPercentage = 0.005;  // 最大损失为最大生命值的0.1%
    float maxLoss = feature.currentMaxHealth * maxLossPercentage;
    if(fitness>0.7){
        return (fitness-0.7)*maxLoss;
    }else if(fitness>0){
        return (fitness-0.7 )*maxLoss;
    }else{
        return maxLoss;
    }
    
}

void Consumer::checkEnvironment(){
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

void Consumer::actReproduction(QuadTreeAtlas& quadTreeAtlas){
    if(feature.reproductionCount<feature.maxReproductionCount){
        feature.reproductionCount+=2;
    }else{
        // if(currentNode.lock()->entities.size()>MAX_ENTITIES){
        //     return;
        // }
        if(feature.currentHuger<feature.maxHunger/3||feature.currentHealth<feature.currentMaxHealth/5){
            return;
        }
        auto newRect=feature.rectInAtlas;
        newRect.x=(newRect.x+4096+RandomUtil::getRandomInt(-140,140))%4096;
        newRect.y=(newRect.y+4096+RandomUtil::getRandomInt(-140,140))%4096;
        auto ent=quadTreeAtlas.getEntityFactory().createEntity(CONSUMER_TYPE,newRect ,quadTreeAtlas.getRoot());
        this->feature.gene.copyGeneTo(ent->getGene());
        quadTreeAtlas.entityToAdd.push_back(ent);
        feature.reproductionCount=0;
        std::cout<<"Create a consumer"<<std::endl;
    }
}

int Consumer::computeDirection(){
        auto currentRect=feature.rectInAtlas;
        auto searchRect=currentRect;
        searchRect.x-=4*currentRect.w;
        searchRect.y-=4*currentRect.h;
        searchRect.h*=8;
        searchRect.w*=8;
        auto tree=currentNode.lock()->quadTree;
        //上 下 左 右
        std::vector<float> directionScore(8, 0.0f);  // 假设有8个方向
        tree.applyToNode(tree.getRoot(),searchRect,[&directionScore,currentRect,this,searchRect](std::shared_ptr<QuadTreeAtlasNode>& node){
            for(auto ent:node->entities){
                int type_score=0;
                if(!intersect_(ent->getRectInAtlas(),searchRect)){
                    continue;
                }
                switch(ent->getEntityType()){
                    case PRODUCER_TYPE:
                    if(this->getFeature().currentHuger<=this->getFeature().maxHunger/2){
                        type_score=4;
                    }else{
                        type_score=2;
                    }
                    break;
                    case CONSUMER_TYPE:
                    //if(this->getFeature().currentHuger>=this->getFeature().maxHunger/2){
                    type_score=-1;
                    //}
                    //type_score=0;
                    
                    break;
                }

                // auto entRect=ent->getFeature().rectInAtlas;
                
                // if(entRect.x < currentRect.x){
                //     directionScore[3]+=type_score;  // 左
                // } else {
                //     directionScore[2]+=type_score;  // 右
                // }
                // //垂直方向
               
                // if(entRect.y < currentRect.y){
                //     directionScore[1]+=type_score;  // 上
                // } else {
                //     directionScore[0]+=type_score;  // 下
                // }
                // // 对角方向
                // if(entRect.x < currentRect.x && entRect.y < currentRect.y){
                //     directionScore[5]+=type_score;  // 左上
                // } else if(entRect.x > currentRect.x && entRect.y < currentRect.y){
                //     directionScore[6]+=type_score;  // 右上
                // } else if(entRect.x > currentRect.x && entRect.y > currentRect.y){
                //     directionScore[4]+=type_score;  // 右下
                // } else if(entRect.x < currentRect.x && entRect.y > currentRect.y){
                //     directionScore[7]+=type_score;  // 左下
                // }
                auto entRect = ent->getFeature().rectInAtlas;
                int x_offset = currentRect.x - entRect.x;  // x方向的偏移量
                int y_offset = currentRect.y - entRect.y;  // y方向的偏移量
                float x_distance = std::abs(x_offset);  // x方向的绝对距离
                float y_distance = std::abs(y_offset);  // y方向的绝对距离
                float x_weight = 100.0f /(1+std::exp(x_distance + 1)); // 防止除以零，x方向的权重
                float y_weight = 100.0f /(1+std::exp(y_distance + 1)); // 防止除以零，y方向的权重
                
                // 水平方向
                if (x_offset > 0) {
                    directionScore[3] += type_score * x_weight;  // left
                } else if (x_offset < 0) {
                    directionScore[2] += type_score * x_weight;  // right
                }

                // 垂直方向
                if (y_offset > 0) {
                    directionScore[1] += type_score * y_weight;  // up
                } else if (y_offset < 0) {
                    directionScore[0] += type_score * y_weight;  // down
                }

                // 对角方向
                // if (x_offset > 0 && y_offset > 0) {
                //     directionScore[7] += type_score * (x_weight + y_weight) / 2;  // 左下
                // } else if (x_offset < 0 && y_offset > 0) {
                //     directionScore[4] += type_score * (x_weight + y_weight) / 2;  // 右下
                // } else if (x_offset < 0 && y_offset < 0) {
                //     directionScore[6] += type_score * (x_weight + y_weight) / 2;  // 右上
                // } else if (x_offset > 0 && y_offset < 0) {
                //     directionScore[5] += type_score * (x_weight + y_weight) / 2;  // 左上
                // }
            }
        });
        int bestDirection = 0;
        // int maxScore = directionScore[0];
        // for (int i = 1; i < 8; i++) {
        //     if (directionScore[i] > maxScore) {
        //         maxScore = directionScore[i];
        //         bestDirection = i;
        //     }
        // }
        float total = std::accumulate(directionScore.begin(), directionScore.end(), 0.0f);
        for (float& freq : directionScore) {
            freq /= total;
        }
        bestDirection=determineBestDirection(directionScore);
        updateMoveHistory(bestDirection);
        randomMove=false;
        
        return bestDirection;
        
        
        
    }

int Consumer::determineBestDirection(std::vector<float>& currentDirectionScores) {
   
    std::vector<float> directionFrequency(8, 0);  // 假设有8个方向

    // 计算历史方向的频率
    for (int dir : moveHistory) {
        directionFrequency[dir] += 1.0;
    }

    // 归一化频率
    float total = std::accumulate(directionFrequency.begin(), directionFrequency.end(), 0.0f);
    for (float& freq : directionFrequency) {
        freq /= total;
    }

    // std::vector<float> currentDirectionScores(8, 0);  // 当前方向得分，假设已计算填充

    // 影响因子设定
    float historyWeight = 0.2;  // 历史数据的权重
    float currentWeight = 0.8;  // 当前数据的权重

    // 结合得分
    std::vector<float> combinedScores(8, 0);
    for (int i = 0; i < 8; i++) {
        combinedScores[i] = historyWeight * directionFrequency[i] + currentWeight * currentDirectionScores[i];
    }

    // 寻找最佳方向
    int bestDirection = std::distance(combinedScores.begin(), std::max_element(combinedScores.begin(), combinedScores.end()));
    return bestDirection;
}

void Consumer::updateMoveHistory(int bestDirection){
    moveHistory.push_back(bestDirection);
    if (moveHistory.size() > 10) {
        moveHistory.pop_front();  // 从前端移除
    }
}
