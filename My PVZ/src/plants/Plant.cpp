#include "plant.h"
#include "Game.h"



Sunshine::Sunshine(int xI, int yI, int valueI,SDL_factory& GUI_factory,SunshineState stateI,SunshineType typeI){
    x=xI,y=yI,value=valueI,is_collected=false,acceleration_y=0;
    textures["sunshine_outer"]=GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_outer"];
    textures["sunshine_ball"]=GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_ball"];
    textures["sunshine_card"]=GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_card"];
    destRect.h=90,destRect.w=90,disappearCount=0,disappearMaxTime=1800;
    destRect.x=x,destRect.y=y;
    gathered_dest.x=10,gathered_dest.y=-10;
    type=typeI,x_off=0,y_off=0;
    state=stateI;
    falling_dest.x=xI,falling_dest.y=yI+350;
    
}

void Sunshine::render(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer,textures["sunshine_outer"],nullptr,&destRect);
    SDL_RenderCopy(renderer,textures["sunshine_ball"],nullptr,&destRect);
}

bool Sunshine::isCollected(int mouseX,int mouseY){
    if(mouseX>destRect.x&&mouseX<destRect.x+destRect.w&&mouseY>destRect.y&&mouseY<destRect.y+destRect.h){
        is_collected=true;
        state=MOVING_TO_SUNSHINEBANK;
        x_off=(falling_dest.x-gathered_dest.x)/30,y_off=(falling_dest.y-gathered_dest.y)/30;
        return true;
    }
    return false;
}

void Plant::loadPlantFeature(const plantTypeInGrid type){
    switch(type){
        case PeaShooterType:
        feature.health=300;
        feature.attackCoolDown=6;
        feature.coolDown=5;
        feature.type=type;
        break;
        case SunflowerType:
        feature.health=300;
        feature.attackCoolDown=0;
        feature.coolDown=5;
        feature.type=type;
        break;
    }
}


void PeaShooter::act(Level& playingLevel){
    if(feature.attackCoolDown>=80&&is_attacking){
        playingLevel.grid[rowPos-1][colPos].bullets.push_back(attack());
        feature.attackCoolDown=0;
    }else{
        feature.attackCoolDown++;
    }
    
}

void PeaShooter::renderPlant(SDL_Renderer* renderer,const BackGroundState state){
    
    
    const int partWidth=24*zoom_rate,partHeight=24*zoom_rate;
    auto currentPos=getPosFromGrid(rowPos,colPos,state);
     // 设置根部的目标矩形
    
    // 设置上部的目标矩形，它应该位于根部的正上方
    SDL_RendererFlip flip = facing_right ? SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL;
        // 渲染纹理
    SDL_RenderCopyEx(renderer, texture["PeaShooterTop"], nullptr, &destRects["top"], 0.0, nullptr, flip);
    SDL_RenderCopyEx(renderer, texture["PeaShooterBase"], nullptr, &destRects["base"], 0.0, nullptr, flip);
}

unique_ptr<PlantBullet> PeaShooter::attack(){
    unique_ptr<PlantBullet> target_ptr=std::make_unique<PlantBullet>(grid_x_pos,(grid_y_pos),120,0,20);
    target_ptr->textures["peaBullet"]=Plant::texture["peaBullet"];
    target_ptr->destRect.x=PeaShooter::destRects["top"].x+10*zoom_rate;
    target_ptr->destRect.y=PeaShooter::destRects["top"].y;
    target_ptr->grid_x_pos=target_ptr->destRect.x;
    target_ptr->grid_y_pos=target_ptr->destRect.y;
    target_ptr->destRect.w=zoom_rate*24,target_ptr->destRect.h=zoom_rate*24;
    target_ptr->rowPos=PeaShooter::rowPos;
    target_ptr->colPos=PeaShooter::colPos;
    target_ptr->collisionRect.x=target_ptr->destRect.x+2*zoom_rate;
    target_ptr->collisionRect.y=target_ptr->destRect.y+2*zoom_rate;
    target_ptr->collisionRect.h=target_ptr->destRect.h-2*zoom_rate;
    target_ptr->collisionRect.w=target_ptr->destRect.w-2*zoom_rate;
    feature.attackCoolDown=0;
    return target_ptr;
}

void PeaShooter::init_plant(const int row,const int col,const BackGroundState state){
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


std::unique_ptr<Plant> PlantFactory::createPlant(const std::string&name,const int row,const int col,const BackGroundState state){
    if(name=="PeaShooter"){
        std::unique_ptr target_ptr=std::make_unique<PeaShooter>();
        target_ptr->texture=_plant_res.plantTextures[PeaShooterType];
        target_ptr->init_plant(row,col,state);
        return target_ptr;
    }else if(name=="Sunflower"){
        std::unique_ptr target_ptr=std::make_unique<Sunflower>();
        target_ptr->texture=_plant_res.plantTextures[SunflowerType];
        target_ptr->init_plant(row,col,state);
        target_ptr->GUI_factory=_plant_res;
        return target_ptr;
    }
    return nullptr;
}

void PeaShooter::testEnemy(const vector<vector<GameGrid>>& grid){
    for(int col=colPos;col<11;col++){
        if(!grid[rowPos-1][col].zombies.empty()){
            is_attacking=true;
            return;
        }
    }
    is_attacking=false;
    return;
}

void PlantBullet::update(const BackGroundState state){
    if(grid_x_pos+velocityX>GRID_LENGTH&&colPos!=12){
        colPos++;
        grid_x_pos=0;
    }else if(grid_x_pos+velocityX<0&&colPos>1){
        colPos--;
        grid_x_pos=1200;
    }else if(grid_x_pos+velocityX>GRID_LENGTH&&colPos==12){
        isActive=false;
    }else if(grid_x_pos+velocityX<0&&colPos==1){
        isActive=false;
    }else if((grid_y_pos-velocityY<0)&&rowPos!=1){ 
        rowPos--;
        grid_y_pos=1200;    
    }else if(grid_y_pos-velocityY>GRID_LENGTH*5&&rowPos<5){
        rowPos++;
        grid_y_pos=0;
    }else if((grid_y_pos-velocityY<0)&&rowPos==1){
        isActive=false;
    }else if(grid_y_pos-velocityY>GRID_LENGTH*5&&rowPos==5){
        isActive=false;
    }else{
        grid_x_pos+=velocityX;
        grid_y_pos-=velocityY;//向上为正，所以这里减
    }
    auto currentPos=getPosFromGrid(rowPos,colPos,state);
    destRect.x = destRect.x+(velocityX*GRID_WIDTH_PIXEL/GRID_LENGTH);
    destRect.y = destRect.y-(velocityY*GRID_HEIGHT_PIXEL/GRID_LENGTH);
    collisionRect.x=destRect.x+10;
    collisionRect.y=destRect.y+10;
}
void PlantBullet::updatePierceAbility(){
    if(pierceCount>MaxPierceNum){
        isActive=false;
    }
}

void PlantBullet::render(SDL_Renderer* renderer,const BackGroundState state){
    cout<<SDL_RenderCopyEx(renderer,textures["peaBullet"],nullptr,&destRect,0.0,nullptr,SDL_FLIP_NONE);
    
}

bool PlantBullet::checkCollision(const SDL_Rect& targetRect){
    if(SDL_HasIntersection(&collisionRect, &targetRect)){
        return true;
    }
    return false;
}



