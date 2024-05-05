#include "Model/EntityFactory.hpp"

std::unique_ptr<Entity> EntityFactory::createEntity(const EntityType type,SDL_Rect RectInAtlas,std::shared_ptr<QuadTreeAtlasNode>& node){
    Point pos(RectInAtlas.x,RectInAtlas.y);
    switch(type){
        case PRODUCER_TYPE:
        id_count++;
        return std::make_unique<Producer>(id_count,type,pos,node);
        break;
        case CONSUMER_TYPE:
        id_count++;
        return std::make_unique<Consumer>(id_count,type,pos,node);
        break;

        default:
        return nullptr;
        break;
    }
}

std::unique_ptr<Entity> EntityFactory::createEntity(const EntityType type,std::shared_ptr<QuadTreeAtlasNode>& node,EntityFeature& feature){

    switch(type){
        case PRODUCER_TYPE:
        id_count++;
        return std::make_unique<Producer>(id_count,feature,node);
        break;
        case CONSUMER_TYPE:
        id_count++;
        return std::make_unique<Consumer>(id_count,feature,node);
        break;

        default:
        return nullptr;
        break;
    }
}