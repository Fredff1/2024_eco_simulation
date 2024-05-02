#ifndef QUAD_TREE_ATLAS_H
#define QUAD_TREE_ATLAS_H

#include "../Util/GlobalEnum.hpp"
#include "../Util/GlobalStruct.hpp"
#include <memory>
#include "Entity.hpp"
#include <utility>
#include <list>
#include <functional>
#include <SDL2/SDL.h>
#include "Model/FrameDateBuffer.hpp"
#include "Model/Consumer.hpp"
#include "Model/Producer.hpp"
#include "Model/WaterFlow.hpp"
#include "Model/EntityFactory.hpp"
#include "Util/GlobalFunc.hpp"
#include "Util/RandomUtil.hpp"
#include "Model/EnvironmentFac.hpp"
#include "model/Resource.hpp"

#define MAX_ENTITIES 25
#define MIN_SIZE 20
#define MAX_TREE_DEPTH 6

class QuadTreeAtlas;

struct QuadTreeAtlasNode{
    /*first x second y in the window 
    The Position in the atlas,not in the GUI*/
    SDL_Rect rectInAtlas;
    std::list<std::shared_ptr<Entity>> entities;
    std::shared_ptr<QuadTreeAtlasNode> children[4];
    std::shared_ptr<QuadTreeAtlasNode> father;
    EnvironmentFactor environmentFactor;
    std::list<AtlasState> currentStates;
    std::list<std::shared_ptr<WaterFlow>> waterFlowList;
    int nodeDepth;
    ResourceInAtlas resource;

    QuadTreeAtlas& quadTree;

    Point getMiddle(){
        return Point((rectInAtlas.x+rectInAtlas.w/2),(rectInAtlas.y+rectInAtlas.h/2));
    }

    QuadTreeAtlasNode(float x,float y,float h,float w,int nodeDepth,QuadTreeAtlas& tree):environmentFactor(0,0,0),quadTree(tree){
        rectInAtlas.x=x,rectInAtlas.y=y,rectInAtlas.w=w,rectInAtlas.h=h;
        father=nullptr;
        this->nodeDepth=nodeDepth;
        resource.setLightResource(rectInAtlas.h*rectInAtlas.w);
    }

    /* Set the father node*/
    void setFather(const std::shared_ptr<QuadTreeAtlasNode>& father){
        this->father=father;
    }
    /*Test if the entity's rect is in the node's rect*/
    bool contains(const Entity& ent) const;

    /* Return if the input rect intersect with the visible rect*/
    bool isRectVisible(const SDL_Rect& rect,const SDL_Rect& visibleRect);

     /* Return if the rect intersects with the node's rect in atlas*/
    bool intersect(const SDL_Rect& rect);


    /* Return if the rect is completely in the node's rect in atlas*/
    bool surroundRect(const SDL_Rect& rect);

    /* Return is the node's rect in atlas is in the rect provided*/
    bool beSurroundedRect(const SDL_Rect& rect);

    /* return is the entity list is empty*/
    bool hasEntity(){
        return !entities.empty();
    }

    

    /* Return its entity list*/
     std::list<std::shared_ptr<Entity>>& getEntities(){
        return entities;
    }

    /* Set the medium node to default state
        and change its childs' state according to itself
        It assumes that the node has childs*/
    void initializeWhenSubDivide(){
        
    }

    
};


class QuadTreeAtlas{
private:
    //first x second y in axis
    std::pair<float,float> positionInWindow;
    //first:h second w
    std::pair<float,float> AtlasSize;
    
    std::shared_ptr<QuadTreeAtlasNode> root;

    std::list<std::shared_ptr<WaterFlow>> waterFlowList;
    int waterFLowIDCount=1;

    SDL_Rect visibleRect;

    EntityFactory entityFactory;

    int currentDepth;

    int entityCount;
    /* Subdivide the tree into four smaller nodes
    Parameters:
    node : The node to be divided*/
    void subdivide(std::shared_ptr<QuadTreeAtlasNode>& node);


    //void initWaterFlow();

    /* Add an Entity to the list of a node
    Parameters:
    entity: the target to be added
    node the target node to add to*/
    void addEntity(std::shared_ptr<Entity>& entity,std::shared_ptr<QuadTreeAtlasNode>& node){
        entity->setCurrentNode(node);
        node->getEntities().push_back(entity);
        currentDepth=1;
    }

    

    /* Move entity automatically to the smallest node that surround the entity
    Warning:it does not erase the previous entity in the node,so please doit yourself
    Parameters:
    entity : the target entity*/
    void moveEntity(std::shared_ptr<Entity>& entity);

public:
    std::vector<std::shared_ptr<Entity>> entityToAdd;

    QuadTreeAtlas(float h,float w):root(std::make_shared<QuadTreeAtlasNode>(0,0,h,w,1,*this)),AtlasSize(h,w){
        //insertEntity(root,entityFactory.createEntity(PRODUCER_TYPE,initRect(600,600,30,30),root));
        initEntities(0,0,60);
        //setWaterFlowToArea(initRect(200,200,3200,3200),VORTEX_WATER_FLOW,3,60000);
    }

    void setEntityCount(const int a){
        entityCount=a;
    }

    EntityFactory& getEntityFactory(){
        return entityFactory;
    }

    std::list<std::shared_ptr<WaterFlow>>& getWaterFlowList(){
        return waterFlowList;
    }

    /*Insert an entity to the best node that suits it
    Parameters:
    node : the rootNode to begin insert
    entity : the target entity to insert*/
    void insertEntity(std::shared_ptr<QuadTreeAtlasNode>& node, std::shared_ptr<Entity>& entity);


    

    /* set its visible rect in the whole atlas*/
    void setVisibleRect(const SDL_Rect& target){
        this->visibleRect=target;
    }

    const SDL_Rect& getVisibleRect(){
        return visibleRect;
    }
    /* Find the node that best suits the given area
    Return the smallest node that surrounds the given area
    Parameters:
    currentRoot: the root to start
    area : the changed area*/
    std::shared_ptr<QuadTreeAtlasNode>& findNode(std::shared_ptr<QuadTreeAtlasNode>&currentRoot, const SDL_Rect& bounds);

    /* find the smallest node that intersect with the area*/
    std::shared_ptr<QuadTreeAtlasNode>& findSmallestNode(std::shared_ptr<QuadTreeAtlasNode>&currentRoot, const SDL_Rect& bounds);

    /* get the root node*/
    std::shared_ptr<QuadTreeAtlasNode>& getRoot(){
        return root;
    }
    

    std::pair<float,float> getPosInWindow(){
        return this->positionInWindow;
    }

    std::pair<float,float> getAtlasSize(){
        return this->AtlasSize;
    }

    void setAtlasSize(float height,float width){
        AtlasSize.first=height;
        AtlasSize.second=width;

    }

    /* Update all the entities in the tree
    Parameters :
    node: need to be the root of*/
    void updateEntities(const std::shared_ptr<QuadTreeAtlasNode>& node);

   
    /*First it updates all the entities' state
      Second it moves the entities that changes its node using moveENtities()
      Third it pushes the entities needing render to the frameDate's list
      ParaMeters:
      node: the root node to start query
      data: the frame data to change*/
    void queryUpdateEntitiesInArea(const std::shared_ptr<QuadTreeAtlasNode>& node, FrameData & data);
        
    /* Apply the input function to all the leaf nodes that intersect the area
    Parameters :
    currentNoe : the node to start
    area : the area for search
    func : the lambda function to apply to all the leaf nodes */
    void applyToNode(std::shared_ptr<QuadTreeAtlasNode>& currentNode, const SDL_Rect& searchRect, std::function<void(std::shared_ptr<QuadTreeAtlasNode>&)> func); 
            
        
    
    /* Add the state to the list of the nodes beSurrounded by the area
    Parameters: 
    area : the area to surround the node
    state : the state to add to the node's state list*/
    void addAtlasNodeState(const SDL_Rect& area,AtlasState state);

    /* Delete the state to the list of the nodes beSurrounded by the area 
    Parameters: 
    area : the area to surround the node
    state : the state to add to the node's state list*/
    void deleteAtlasState(const SDL_Rect& area,AtlasState state);
    
    /* Set the smallest node that surrounds the area the given flow type
    angle is useless when setting an vortex*/
    void setWaterFlowToArea(const SDL_Rect& area,FlowType type,int flowDirAngle,int flowSpeed,long life);

    /* Set the smallest node that surrounds the area the given flow type
    angle is useless when setting an vortex*/
    void setWaterFlowToArea(const SDL_Rect& area,FlowType type,int flowDirAngle,long life);
       
    /*Update the atlas state according in each frame*/
    void updateAtlasNodes(const std::shared_ptr<QuadTreeAtlasNode>& node,FrameData& data);


    void updateUniversal(FrameData& data);

    /* All the update is in this func*/
    void update(FrameData& data);


    void addToFrameData(std::shared_ptr<QuadTreeAtlasNode>& node,FrameData& data);

    /* Automatically insert water flow and gives it an id*/
    void insertWaterFlow(WaterFlow& waterFLow);

    /* Return the shared_ptr of the entity using the id.If not found,return nullptr*/
    std::shared_ptr<Entity> findEntity( std::shared_ptr<QuadTreeAtlasNode>& node,int id);

    void removeAllEntities(std::shared_ptr<QuadTreeAtlasNode>& node);

    /* Randomly insert much entities to the atlas*/
    void initEntities(int consumerNun,int producerNum,int decomposerNum);

    /* Delete all atlas node except root node*/
    void resetAtlas(std::shared_ptr<QuadTreeAtlasNode>& node);
};





#endif