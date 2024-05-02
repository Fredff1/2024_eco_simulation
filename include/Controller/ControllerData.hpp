#ifndef CONTROLLER_DATA
#define CONTROLLER_DATA
#include "ControllerEvents.hpp"

struct ControllerData{
    ControllerEvent event;
    SDL_Rect visibleRect;
    ControllerData():event(NO_CONTROLLER_ACTION){

    }
    ~ControllerData(){}
};












#endif