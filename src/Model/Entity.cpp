#include "Entity.hpp"
#include "Model/QuadTreeAtlas.hpp"

Entity::Entity(int id,EntityType type,Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):currentNode(currentNode){
    feature.id=id;
    feature.type=type;
}

Entity::Entity(int id,EntityFeature& feature,std::shared_ptr<QuadTreeAtlasNode>& currentNode):feature(feature),currentNode(currentNode){
    feature.id=id;
}

void Entity::setCurrentNode(std::shared_ptr<QuadTreeAtlasNode> targetNode){
    currentNode.operator=(targetNode);
}