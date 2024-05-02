#include "Model/EntityFactory.hpp"

std::shared_ptr<Entity> EntityFactory::createEntity(const EntityType type,SDL_Rect RectInAtlas,std::shared_ptr<QuadTreeAtlasNode>& node){
        Point pos(RectInAtlas.x,RectInAtlas.y);
        switch(type){
            case PRODUCER_TYPE:
            id_count++;
            return std::make_shared<Producer>(id_count,type,pos,node);
            break;
            case CONSUMER_TYPE:
            id_count++;
            return std::make_shared<Consumer>(id_count,type,pos,node);
            break;

            default:
            return nullptr;
            break;
        }
    }