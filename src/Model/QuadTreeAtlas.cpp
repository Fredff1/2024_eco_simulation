#include "QuadTreeAtlas.hpp"
#include <iostream>
#include "Util/ThreadSafeCout.hpp"

bool QuadTreeAtlasNode::contains(const Entity& ent) const{
    int x=ent.getRectInAtlas().x;
    int y=ent.getRectInAtlas().y;
    return x>rectInAtlas.x&&y>rectInAtlas.y&&x<rectInAtlas.x+rectInAtlas.w&&y<rectInAtlas.y+rectInAtlas.h;
}

bool QuadTreeAtlasNode::isRectVisible(const SDL_Rect& rect,const SDL_Rect& visibleRect){
    if (rect.x + rect.w <= visibleRect.x || visibleRect.x + visibleRect.w <= rect.x) {
        return false;
    }
    
    // Check if one rectangle is above the other
    if (rect.y + rect.h <= visibleRect.y || visibleRect.y + visibleRect.h <= rect.y) {
        return false;
    }
    return true;
}

bool QuadTreeAtlasNode::intersect(const SDL_Rect& rect){
    if (rect.x + rect.w < rectInAtlas.x || rectInAtlas.x + rectInAtlas.w < rect.x) {
        return false;
    }
    
    // Check if one rectangle is above the other
    if (rect.y + rect.h < rectInAtlas.y || rectInAtlas.y + rectInAtlas.h < rect.y) {
        return false;
    }
    return true;
}

bool QuadTreeAtlasNode::surroundRect(const SDL_Rect& rect){
    bool flag_1=rectInAtlas.x<=rect.x&&rectInAtlas.y<=rect.y;
    bool flag_2=(rectInAtlas.x+rectInAtlas.w)>=(rect.x+rect.w);
    bool flag_3=(rectInAtlas.y+rectInAtlas.h)>=(rect.y+rect.h);
    return flag_1&&flag_2&&flag_3;
}

bool QuadTreeAtlasNode::beSurroundedRect(const SDL_Rect& rect){
    bool flag_1=rectInAtlas.x>=rect.x&&rectInAtlas.y>=rect.y;
    bool flag_2=(rectInAtlas.x+rectInAtlas.w)<=(rect.x+rect.w);
    bool flag_3=(rectInAtlas.y+rectInAtlas.h)<=(rect.y+rect.h);
    return flag_1&&flag_2&&flag_3;
}


void QuadTreeAtlas::insertEntity(std::shared_ptr<QuadTreeAtlasNode>& node, std::unique_ptr<Entity>& entity) {
    //std::cout << "Attempting to insert Entity ID: " << entity->getID() << " at Node Depth: " << node->nodeDepth << std::endl;
    if (!node->intersect(entity->getRectInAtlas())) {
        //std::cout << "Entity ID: " << entity->getID() << " does not intersect with Node. Insertion skipped." << std::endl;
        return;
    }

    if (!node->children[0]) {
        if (node->entities.size() < MAX_ENTITIES || node->nodeDepth >= MAX_TREE_DEPTH) {
            entity->setCurrentNode(node);
            node->entities.push_back(std::move(entity));
            //std::cout << "Entity ID: " << entity->getID() << " inserted directly into Node." << std::endl;
        } else {
            subdivide(node);
            //std::cout << "Node subdivided due to capacity/depth limit. Re-attempting insertion." << std::endl;
            insertEntity(node, entity);
        }
    } else {
        bool inserted = false;
        for (auto& child : node->children) {
            if (child->intersect(entity->getRectInAtlas())) {
                insertEntity(child, entity);
                inserted = true;
                //std::cout << "Entity ID: " << entity->getID() << " passed to child node." << std::endl;
                break;
            }
        }
        if (!inserted) {
            for (auto& child : node->children) {
                bool flag;
                flag=child->intersect(entity->getRectInAtlas());
                    
            }
            entity->setCurrentNode(node);
            node->entities.push_back(std::move(entity));
            //std::cout << "Entity ID: " << entity->getID() << " kept at current node as no child node was suitable." << std::endl;
        }
    }
}


void QuadTreeAtlas::subdivide(std::shared_ptr<QuadTreeAtlasNode>& node) {
    float halfWidth = node->rectInAtlas.w / 2.0;
    float halfHeight = node->rectInAtlas.h / 2.0;
    float node_x = node->rectInAtlas.x;
    float node_y = node->rectInAtlas.y;
    int change=0;
    node->children[0] = std::make_shared<QuadTreeAtlasNode>(node_x-change, node_y-change, halfWidth+change, halfHeight+change, node->nodeDepth + 1,*this);
    node->children[1] = std::make_shared<QuadTreeAtlasNode>(node_x-change + halfWidth, node_y-change, halfWidth+change, halfHeight+change, node->nodeDepth+ 1,*this);
    node->children[2] = std::make_shared<QuadTreeAtlasNode>(node_x-change, node_y + halfHeight-change, halfWidth+change, halfHeight+change, node->nodeDepth + 1,*this);
    node->children[3] = std::make_shared<QuadTreeAtlasNode>(node_x-change + halfWidth, node_y + halfHeight-change, halfWidth+change, halfHeight+change, node->nodeDepth + 1,*this);
    
    
    node->initializeWhenSubDivide();
    auto it = node->entities.begin();
    while (it != node->entities.end()) {
        
        bool moved = false;
        for (auto& child : node->children) {
            if (child->intersect((*it)->getRectInAtlas())) {
                (*it)->setCurrentNode(child);
                child->entities.push_back(std::move(*it));
                it = node->entities.erase(it);
                moved = true;
                break;
            }
        }
        if (!moved) {
            ++it;
        }
    }

    auto waterIt=node->waterFlowList.begin();
    while (waterIt != node->waterFlowList.end()) {
        auto flow = *waterIt;

        for (auto& child : node->children) {
            child->waterFlowList.push_back(flow);
            if (intersect_(flow->feature.targetArea,child->rectInAtlas)) {
                //child->waterFlowList.push_back(flow);
                //ent->setCurrentNode(child);
                
            }
        }
        waterIt = node->waterFlowList.erase(waterIt);
        
    }
    
}


std::shared_ptr<QuadTreeAtlasNode>& QuadTreeAtlas::findNode(std::shared_ptr<QuadTreeAtlasNode>& currentRoot, const SDL_Rect& bounds) {
    
    if (!currentRoot->intersect(bounds)) {
        return currentRoot;
    }

    if (currentRoot->children[0]) {
        for (auto& child : currentRoot->children) {
            if (child->surroundRect(bounds)) {
                return findNode(child, bounds);
            }
        }
    }
    return currentRoot;
}

std::shared_ptr<QuadTreeAtlasNode>& QuadTreeAtlas::findSmallestNode(std::shared_ptr<QuadTreeAtlasNode>&currentRoot, const SDL_Rect& bounds){
    if (!currentRoot->intersect(bounds)) {
        return currentRoot;
    }

    if (currentRoot->children[0]) {
        for (auto& child : currentRoot->children) {
            if (child->intersect(bounds)) {
                return findSmallestNode(child, bounds);
            }
        }
    }
    return currentRoot;
}


void QuadTreeAtlas::moveEntity(std::unique_ptr<Entity>& entity) {
    auto current = entity->getCurrentNode().lock();
    if (!current) return;

    auto newNode = findSmallestNode(root, entity->getRectInAtlas());
    if (newNode != current) {
        current->entities.remove(entity);
        insertEntity(newNode, entity);
    }
}


void QuadTreeAtlas::queryUpdateEntitiesInArea(const std::shared_ptr<QuadTreeAtlasNode>& node, FrameData & data) {
    if (!node) return;

    if (node->children[0]) {
        // 遍历子节点
        for (int i = 0; i < 4; i++) {
            queryUpdateEntitiesInArea(node->children[i], data);
        }
    } else {
        // 遍历当前节点的实体
        std::vector<std::unique_ptr<Entity>> toMove;
        auto& entities = node->getEntities();
        auto it = entities.begin();
        while (it != entities.end()) {
            auto& ent = *it;
            if (!node->intersect(ent->getRectInAtlas())&&(*it)->getAlive()==true) {
                // 需要移动的实体添加到临时列表
                toMove.push_back(std::move(*it));
                it = entities.erase(it); // 安全删除当前实体
                
            } else if((*it)->getAlive()==false){
                it=entities.erase(it);
                
            }else {
                // 检查实体是否在可见区域内
                //if (node->isRectVisible(ent->getRectInAtlas(), visibleRect)) {
                //data.entitiesInAtlas.push_back(ent->getEntityState());
               // }
                ++it;
            }
        }

        // 处理需要移动的实体
        for (auto& ent : toMove) {
            //if (node->isRectVisible(ent->getRectInAtlas(), visibleRect)) {
            //data.entitiesInAtlas.push_back(ent->getEntityState());
            //}
            moveEntity(ent);
        }

        auto entIt=entityFactory.entityToAdd.begin();
        while(entIt!=entityFactory.entityToAdd.end()){
            insertEntity(root,(*entIt));
            entIt=entityFactory.entityToAdd.erase(entIt);
        }
    }
}


void QuadTreeAtlas::initEntities(int consumerNun,int producerNum,int decomposerNum){
    for(int i=0;i<producerNum;i++){
        int a=RandomUtil::getRandomInt(-1000,3000);
        int b=RandomUtil::getRandomInt(-1000,3000);
        auto entity=entityFactory.createEntity(PRODUCER_TYPE,initRect(1000+a,1000+b,5,5),root);
        insertEntity(root,entity);
    }
    for(int i=0;i<consumerNun;i++){
        int a=RandomUtil::getRandomInt(-1000,3000);
        int b=RandomUtil::getRandomInt(-1000,3000);
        auto entity=entityFactory.createEntity(CONSUMER_TYPE,initRect(1000+a,1000+b,5,5),root);
        insertEntity(root,entity);
    }
    // for(int i=0;i<producerNum;i++){
    //     int a=RandomUtil::getRandomInt(-100,200);
    //     int b=RandomUtil::getRandomInt(-100,200);
    //     auto entity=entityFactory.createEntity(PRODUCER_TYPE,initRect(700+a,700+b,5,5),root);
    //     insertEntity(root,entity);
    // }
}

void QuadTreeAtlas::updateEntities(const std::shared_ptr<QuadTreeAtlasNode>& node){
    if(node->children[0]){
        if(node->entities.empty()==false){
            std::cout<<"Error for node at depth :"<<node->nodeDepth<<std::endl;
            std::cout<<"Entity number un handled : "<<node->entities.size();
            for(auto& ent:node->entities){
                std::cout<<"Entity type: "<<ent->getEntityType()<<" ";
                std::cout<<"Position: "<<ent->getRectInAtlas().x<<" "<<ent->getRectInAtlas().y<<" ";
            }
            std::cout<<std::endl;
            for(auto& ent:node->entities){
                insertEntity(root,ent);
            }
            node->entities.clear();
            std::cout<<"Error automatically handled"<<std::endl;
        }
        for(int i=0;i<4;i++){
            updateEntities(node->children[i]);
        }
        
        
    }else{
        auto& entities=node->getEntities();
        auto it=node->getEntities().begin();
        while(it!=node->getEntities().end()){
            entityCount++;
            (*it)->update(*this); /* Warning this has bug*/
            //(*it)->updateNew();
            // int count=(*it).use_count();
            // if(count>1){
            //     std::cout<<count<<"for this ent"<<std::endl;
            // }
            //std::cout<<"Entity ID: "<<(*it)->getFeature().id<<"\n";
            it++;
        }
    }
    
}

void QuadTreeAtlas::applyToNode(std::shared_ptr<QuadTreeAtlasNode>& currentNode, const SDL_Rect& searchRect, std::function<void(std::shared_ptr<QuadTreeAtlasNode>&)> func) {
    // 找到最合适的节点
    auto suitableNode = findNode(currentNode, searchRect);
    if (suitableNode) {
        // 遍历这个节点的所有子节点
        if (!suitableNode->children[0]) {  // 检查这是一个叶子节点
            // 如果是叶节点，直接应用函数
            func(suitableNode);
        } else {
            // 如果不是叶节点，对每个子节点应用lambda函数
            for (int i = 0; i < 4; ++i) {
                applyToNode(suitableNode->children[i],searchRect,func);
            }
        }
    }
}

void QuadTreeAtlas::addAtlasNodeState(const SDL_Rect& area,AtlasState state){
    applyToNode(root,area,[area,state](std::shared_ptr<QuadTreeAtlasNode>& node) {
        if(node->beSurroundedRect(area)){
            node->currentStates.push_back(state);
        }
    });
}

void QuadTreeAtlas::deleteAtlasState(const SDL_Rect& area,AtlasState state){
    applyToNode(root,area,[area,state](std::shared_ptr<QuadTreeAtlasNode>& node) {
        if(node->beSurroundedRect(area)){
            node->currentStates;
            auto it =node->currentStates.begin();
            while(it!=node->currentStates.end()){
                if((*it)==state){
                    it=node->currentStates.erase(it);
                }else{
                    ++it;
                }
            }
        }
    });
}

//void QuadTreeAtlas::initWaterFlow(){
    
//}

void QuadTreeAtlas::setWaterFlowToArea(const SDL_Rect& area,FlowType type,int flowDirAngle,int flowSpeed,long life){   
    std::shared_ptr<WaterFlow> newWaterFlow=std::make_shared<WaterFlow>(area,flowDirAngle,flowSpeed,life,waterFLowIDCount++);
    applyToNode(root,area,[area,newWaterFlow](std::shared_ptr<QuadTreeAtlasNode>& node){
        if(node->intersect(area)){
            node->waterFlowList.push_back(newWaterFlow);
        }
    });
    waterFlowList.push_back(newWaterFlow);
}

void QuadTreeAtlas::setWaterFlowToArea(const SDL_Rect& area,FlowType type,int flowSpeed,long life){
    std::shared_ptr<WaterFlow> newWaterFlow=std::make_shared<WaterFlow>(area,flowSpeed,life,waterFLowIDCount++);
    applyToNode(root,area,[area,newWaterFlow](std::shared_ptr<QuadTreeAtlasNode>& node){
        if(node->intersect(area)){
            node->waterFlowList.push_back(newWaterFlow);
        }
    });
    waterFlowList.push_back(newWaterFlow);
}

void QuadTreeAtlas::updateAtlasNodes(const std::shared_ptr<QuadTreeAtlasNode>& node,FrameData& data){
    if (!node) return;

    if (node->children[0]) {
        // 遍历子节点
        for (int i = 0; i < 4; i++) {
            updateAtlasNodes(node->children[i], data);
        }
    } else {
        /* update water flow*/
        auto flowIt=node->waterFlowList.begin();
        while (flowIt!=node->waterFlowList.end()){
            
            if((*flowIt)->isActive()){
                
                flowIt++;
            }else{
                flowIt=node->waterFlowList.erase(flowIt);
            }
        }
        /* update other data */
        node->resource.setLightResource(node->rectInAtlas.h*node->rectInAtlas.w/40);
    }
}

void QuadTreeAtlas::updateUniversal(FrameData& data){
    /* Remove water flow from all*/
    auto flowIt=this->waterFlowList.begin();
    while (flowIt!=this->waterFlowList.end()){
        (*flowIt)->update(data);
        if((*flowIt)->isActive()){
            flowIt++;
        }else{
            flowIt=this->waterFlowList.erase(flowIt);
        }
    }
}

void QuadTreeAtlas::update(FrameData& data){
    setEntityCount(0);
    updateUniversal(data);
    updateAtlasNodes(root,data);
    updateEntities(root); /* Warning for this func (cause crash)*/
    queryUpdateEntitiesInArea(root,data);
    addToFrameData(root,data);
}

void QuadTreeAtlas::addToFrameData(std::shared_ptr<QuadTreeAtlasNode>& node,FrameData& data){
    if(node->children[0]){
        for(int i=0;i<4;i++){
            addToFrameData(node->children[i],data);
        }
    }else{
        for(auto& ent:node->entities){
            
            int size=ent->getRectInAtlas().h;
            if(ent->getFeature().rectInAtlas.h>50){
                std::cout<<"Error size for entity type "<<ent->getFeature().type<<std::endl;
                std::cout<<"error size for state"<<ent->getFeature().rectInAtlas.h<<std::endl;
                
            }
            data.entitiesInAtlas.push_back(ent->getFeature());
        }
    }
}

void QuadTreeAtlas::insertWaterFlow(WaterFlow& waterFLow){
    auto area=waterFLow.feature.targetArea;
    std::shared_ptr<WaterFlow> newWaterFlow=std::make_shared<WaterFlow>(waterFLow);
    newWaterFlow->setID(waterFLowIDCount++);
    
    applyToNode(root,area,[area,newWaterFlow](std::shared_ptr<QuadTreeAtlasNode>& node){
        if(node->intersect(area)){
            node->waterFlowList.push_back(newWaterFlow);
        }
    });
    waterFlowList.push_back(newWaterFlow);
}

Entity* QuadTreeAtlas::findEntity(std::shared_ptr<QuadTreeAtlasNode>& node,int id){
    if(!node){
        return nullptr;
    }

    if(!node->children[0]){
        for(auto& ent:node->entities){
            if(ent->getFeature().id==id){
                return ent.get();
            }
        }
    }else{
        for(int i=0;i<4;i++){
            auto ent=findEntity(node->children[i],id);
            if(ent){
                return ent;
            }
        }
    }
    return nullptr;
}

void QuadTreeAtlas::removeAllEntities(std::shared_ptr<QuadTreeAtlasNode>& node){
    if(!node){
        return;
    }

    node->entities.clear();

    if(node->children[0]){
        for(int i=0;i<4;i++){
            removeAllEntities(node->children[i]);
        }
    }
    ThreadSafeCout()<<"All Entities removed\n";
}

void QuadTreeAtlas::resetAtlas(std::shared_ptr<QuadTreeAtlasNode>& node){
    if(!node){
        return;
    }
    

    if(node->children[0]){
        for(int i=0;i<4;i++){
            resetAtlas(node->children[i]);
        }
        if(node->nodeDepth>1){
            node.reset();
        }
        
    }else{
        node.reset();
    }
}