#include "Entity.hpp"
#include "Model/QuadTreeAtlas.hpp"

Entity::Entity(int id,EntityType type,Point position,std::shared_ptr<QuadTreeAtlasNode>& currentNode):id(id),type(type),position(position),state(id,position,type),currentNode(currentNode){

}


void Entity::setCurrentNode(std::shared_ptr<QuadTreeAtlasNode>& targetNode){
    currentNode.operator=(targetNode);
}