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


    EntityFactory(const EntityFactory&) = delete; // 删除复制构造函数
    EntityFactory& operator=(const EntityFactory&) = delete; // 删除复制赋值运算符

    std::vector<std::unique_ptr<Entity>> entityToAdd;

    std::unique_ptr<Entity> createEntity(const EntityType type,SDL_Rect RectInAtlas,std::shared_ptr<QuadTreeAtlasNode>& node);

    std::unique_ptr<Entity> createEntity(const EntityType type,std::shared_ptr<QuadTreeAtlasNode>& node,EntityFeature& feature);
};



















#endif 