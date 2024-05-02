#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "Entity.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"


class EntityFactory{
private:
    int id_count=0;
public:
    EntityFactory(){}

    std::shared_ptr<Entity> createEntity(const EntityType type,SDL_Rect RectInAtlas,std::shared_ptr<QuadTreeAtlasNode>& node);

    std::shared_ptr<Entity> createEntity(const EntityType type,std::shared_ptr<QuadTreeAtlasNode>& node,EntityFeature& feature);
};


















#endif 