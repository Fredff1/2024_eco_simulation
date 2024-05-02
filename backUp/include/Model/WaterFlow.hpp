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
class WaterFlow {
public:
    
    long life;
    int maxLife;
    FlowType type;
    WaterVector parameters;  // 用于存储速度、方向或中心位置等
    SDL_Rect targetArea;

    WaterFlow(){
        type=NO_WATER_FLOW;
        life=0;
    }

    WaterFlow(int lifeLong){
        life=0;
        this->maxLife=lifeLong;
        type=NO_WATER_FLOW;
    }

    

    // 计算给定位置的水流
    WaterVector calculateFlowAt(const int x,const int y) {
        if (type == LINEAR_WATER_FLOW) {
            // 线性水流：parameters.x 是角度，parameters.y 是速度
            float angle = parameters.x;
            float speed = parameters.y;
            return WaterVector(cos(angle) * speed, sin(angle) * speed);
        } else if(type==VORTEX_WATER_FLOW){
            // 旋涡水流：parameters 表示中心位置，固定旋转速度
            int dx = x - parameters.x;
            int dy = y - parameters.y;
            float distance = sqrt(dx * dx + dy * dy);
            float speed = 3 / distance;  // 速度与距离成反比
            if(speed>50){
                speed=50;
            }
            return WaterVector(-dy * speed, dx * speed);  // 顺时针旋转
        } else if(type == NO_WATER_FLOW) {
            return WaterVector(0,0);
        }
        return WaterVector(0,0);
    }

    void setLinearFlow(const SDL_Rect& area,const int flowDirAngle,const int flowSpeed){
        parameters.x=flowDirAngle;
        parameters.y=flowSpeed;
        targetArea=area;
        type=LINEAR_WATER_FLOW;
    }

    void setVortexFlow(const SDL_Rect& area,const int flowSpeed){
        parameters.x=area.x+area.w/2;
        parameters.y=area.y+area.h/2;
        targetArea=area;
        type=VORTEX_WATER_FLOW;
    }

    void update(FrameData& data){
        this->life+=1;
    }

    bool isActive(){
        return this->life<=this->maxLife;
    }
};


#endif 
