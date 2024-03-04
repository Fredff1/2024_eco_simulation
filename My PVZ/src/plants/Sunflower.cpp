#include "Plant.h"
#include "Game.h"

void Sunflower::renderPlant(SDL_Renderer* renderer,const BackGroundState state){
    const int partWidth=24*zoom_rate,partHeight=24*zoom_rate;
    auto currentPos=getPosFromGrid(rowPos,colPos,state);
     // 设置根部的目标矩形
    
    // 设置上部的目标矩形，它应该位于根部的正上方
    SDL_RendererFlip flip = facing_right ? SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL;
        // 渲染纹理
    SDL_RenderCopyEx(renderer, texture["SunflowerTop"], nullptr, &destRects["top"], 0.0, nullptr, flip);
    SDL_RenderCopyEx(renderer, texture["SunflowerBase"], nullptr, &destRects["base"], 0.0, nullptr, flip);
}

void Sunflower::act(Level& playingLevel){
    act_add_sunshine(playingLevel);
}

void Sunflower::act_add_sunshine(Level& playingLevel){
    if(GenerateCount<=GenerateMaxCount){
        GenerateCount++;
    }else if(GenerateCount>GenerateMaxCount){
        GenerateCount=0;
        playingLevel.sunshineList.push_back(generateSunshine());
    }
}

void Sunflower::testEnemy(const vector<vector<GameGrid>>& grid){
    is_attacking=false;
}

void Sunflower::init_plant(const int row,const int col,const BackGroundState state){
    int partWidth=24*zoom_rate,partHeight=24*zoom_rate;
    rowPos=row+1;
    colPos=col;
    grid_y_pos=(GRID_LENGTH)/3;
    grid_x_pos=GRID_LENGTH/6;
    auto currentPos=getPosFromGrid(rowPos,col,state);
    int baseX = currentPos.first+(grid_x_pos*GRID_WIDTH_PIXEL/GRID_LENGTH);
    int baseY = currentPos.second-(grid_y_pos*GRID_HEIGHT_PIXEL/GRID_LENGTH);
    destRects["base"]={baseX, baseY, partWidth, partHeight};
    destRects["top"]={baseX, baseY-10*zoom_rate , partWidth, partHeight};
    collisionRects["base"]={baseX, baseY, partWidth, partHeight};
    collisionRects["top"]={baseX+zoom_rate, baseY-10*zoom_rate , partWidth, partHeight};
}

Sunshine Sunflower::generateSunshine(){
    Sunshine target_sunshine(destRects["top"].x,destRects["top"].y,25,GUI_factory,PLANT_GENERATED,NORMALSUNSHINE);
    target_sunshine.acceleration_y=2,target_sunshine.y_off=-15;
    target_sunshine.falling_dest.y=target_sunshine.destRect.y+40;
    return target_sunshine;
}