#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <vector>
#include "Model/Entity.hpp"

struct FrameData{
public:
    /* the total size of the quadTree */
    float h;
    float w;

    std::vector<EntityRenderData> entitiesInAtlas;
    
    //insert an entity state foe ui to read
    void insertEntities(Entity &ent){
        entitiesInAtlas.push_back(ent.generateEntityRenderData());
    }
};




#endif