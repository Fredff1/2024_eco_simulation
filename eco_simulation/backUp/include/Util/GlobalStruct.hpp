#ifndef GLOBAL_STRUCT_H
#define GLOBAL_STRUCT_H
#include "GlobalEnum.hpp"
#include <SDL2/SDL.h>

struct Point{
    float x;
    float y;
    Point(int x,int y):x(x),y(y){}
    Point(){
        x=0,y=0;
    }
};









#endif