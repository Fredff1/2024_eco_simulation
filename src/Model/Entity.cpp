#include "Entity.hpp"
#include "Model/QuadTreeAtlas.hpp"

Entity::Entity(int id,EntityType type,Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode,EntityFeatureInitMsg msg):currentNode(currentNode){
    feature.id=id;
    feature.type=type;
    feature=EntityFeature(msg);
}

Entity::Entity(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode):feature(feature),currentNode(currentNode){
    feature.id=id;
}

void Entity::setCurrentNode(std::shared_ptr<QuadTreeAtlasNode> targetNode){
    currentNode.operator=(targetNode);
}

EntityRenderData::EntityRenderData(EntityFeature& feature){
    this->id=feature.id;
    this->rectInAtlas=feature.rectInAtlas;
    this->movingAngle=feature.movingFeature.movingAngle;
    this->geneMutateRate=feature.gene.getMutateRate();
    this->isAlive=feature.isAlive;
    this->type=feature.type;
}