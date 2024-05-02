#ifndef WATER_FLOW_H
#define WATER_FLOW_H
#include <SDL2/SDL.h>
#include <math.h>
#include <Model/FrameDateBuffer.hpp>

enum FlowType { 
    NO_WATER_FLOW,
    LINEAR_WATER_FLOW, 
    VORTEX_WATER_FLOW 
};

struct WaterVector{
    float x;
    float y;
    WaterVector(float x,float y):x(x),y(y){}
    WaterVector():x(0),y(0){}
};

struct WaterFlowFeature{
    long life=0;
    int maxLife;
    FlowType type;
    WaterVector parameters;  // 用于存储速度、方向或中心位置等
    SDL_Rect targetArea;
    int id=0;

    /* for linear waterFLow*/
    WaterFlowFeature(const SDL_Rect& targetRect,const int flowDirAngle,const int flowSpeed,const int lifeIn,const int id):id(id){
        parameters.x=flowDirAngle;
        parameters.y=flowSpeed;
        targetArea=targetRect;
        type=LINEAR_WATER_FLOW;
        maxLife=lifeIn;
        
    }

    WaterFlowFeature(const SDL_Rect& targetRect,const int flowSpeed,const int lifeIn,const int id):id(id){
        parameters.x=targetRect.x+targetRect.w/2;
        parameters.y=targetRect.y+targetRect.h/2;
        targetArea=targetRect;
        type=VORTEX_WATER_FLOW;
        maxLife=lifeIn;
    }
};
class WaterFlow {
public:
    
    WaterFlowFeature feature;

    WaterFlow(const SDL_Rect& targetRect,const int flowSpeed,const int lifeIn,int id):feature(targetRect,flowSpeed,lifeIn,id){

        
    }

    WaterFlow(const SDL_Rect& targetRect,const int flowDirAngle,const int flowSpeed,const int lifeIn,int id):feature(targetRect,flowDirAngle,flowSpeed,lifeIn,id){

    }

    WaterFlow(WaterFlowFeature& targetFeature,const int id):feature(targetFeature){
        feature.id=id;
    }

    

    // 计算给定位置的水流
    WaterVector calculateFlowAt(const int x,const int y) {
        
        if (feature.type == LINEAR_WATER_FLOW) {
            // 线性水流：parameters.x 是角度，parameters.y 是速度
            float angle = feature.parameters.x;
            float speed = feature.parameters.y;
            return WaterVector(cos(angle) * speed, sin(angle) * speed);
        } else if(feature.type==VORTEX_WATER_FLOW){
            // 旋涡水流：parameters 表示中心位置，固定旋转速度
            int dx = x - feature.parameters.x;
            int dy = y - feature.parameters.y;
            float distance = sqrt(dx * dx + dy * dy);
            float speed = 3 / distance;  // 速度与距离成反比
            if(speed>50){
                speed=50;
            }
            return WaterVector(-dy * speed, dx * speed);  // 顺时针旋转
        } else if(feature.type == NO_WATER_FLOW) {
            return WaterVector(0,0);
        }
        return WaterVector(0,0);
    }

    void setLinearFlow(const SDL_Rect& area,const int flowDirAngle,const int flowSpeed){
        feature.parameters.x=flowDirAngle;
        feature.parameters.y=flowSpeed;
        feature.targetArea=area;
        feature.type=LINEAR_WATER_FLOW;
    }

    void setVortexFlow(const SDL_Rect& area,const int flowSpeed){
        feature.parameters.x=area.x+area.w/2;
        feature.parameters.y=area.y+area.h/2;
        feature.targetArea=area;
        feature.type=VORTEX_WATER_FLOW;
    }

    void update(FrameData& data){
        feature.life+=1;
    }

    bool isActive(){
        return this->feature.life<=this->feature.maxLife;
    }

    void setID(int idInput){
        this->feature.id=idInput;
    }
};


#endif 
