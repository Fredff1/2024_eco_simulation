#ifndef GLOBAL_FUNC_H
#define GLOBAL_FUNC_H

#include "SDL2/SDL.h"

inline SDL_Rect initRect(int x,int y,int h,int w){
    SDL_Rect target;
    target.x=x,target.y=y;
    target.h=h,target.w=w;
    return target;
}

inline bool intersect_(const SDL_Rect& rect,const SDL_Rect& rect_2){
    if (rect.x + rect.w < rect_2.x || rect_2.x + rect_2.w < rect.x) {
        return false;
    }
    
    // Check if one rectangle is above the other
    if (rect.y + rect.h < rect_2.y || rect_2.y + rect_2.h < rect.y) {
        return false;
    }
    return true;
}

#endif