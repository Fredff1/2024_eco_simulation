#include "Zombie.h"
#include "Game.h"


ConeHeadZombie::ConeHeadZombie(int health,float speed,ZombieType type_input){
    // 为跳动效果的参数设置一些随机的初始值
    jumpHeight = 10.0f + static_cast<float>(rand() % 20); // 在5到10之间随机选择振幅
    jumpFrequency = 10.0f + static_cast<float>(rand() % 5) / 1.0f; // 在0.1到0.2之间随机选择频率
    SDL_Rect BodyRect,HeadRect;
    collisionRectMap["head"]=HeadRect;
    collisionRectMap["body"]=BodyRect;
    zoom_rate=7,zombieState=MOVING;
    loadZombieFeature(ConeHeadZombieType);
    ZombieProtective ConeHead;
    ConeHead.health=400,ConeHead.is_pierce_available=false;
    protectiveList.push_back(ConeHead);
}


void ConeHeadZombie::move(){
    if((grid_x_pos-feature.speed)<0&&colPos!=1){
        colPos-=1;
        grid_x_pos=GRID_LENGTH;
    }else if((grid_x_pos-feature.speed)<0&&colPos==1){   
        exit(1);
    }else{
        grid_x_pos-=feature.speed;
    }
    float cycle = SDL_GetTicks() / 1000.0f * jumpFrequency; // 使用SDL_GetTicks()获取周期性变化的参数
    grid_y_pos = baseYPos + sin(cycle) * jumpHeight; // 在基础位置上应用正弦波形的偏移
}

void ConeHeadZombie::render_zombie(SDL_Renderer* renderer,const BackGroundState state) {
    // 更新destRect的位置
    SDL_RendererFlip flip = facing_left ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    auto currentPos=getPosFromGrid(rowPos,colPos,state);
    switch(zombieState){
        case MOVING: 
        case ATTACKING:
        destRect.x = currentPos.first+(grid_x_pos*GRID_WIDTH_PIXEL/GRID_LENGTH)-GRID_WIDTH_PIXEL;
        destRect.y = currentPos.second+(grid_y_pos*GRID_HEIGHT_PIXEL/GRID_LENGTH)-GRID_HEIGHT_PIXEL;
        // 渲染纹理
        if(!protectiveList.empty()){
            SDL_RenderCopyEx(renderer, texture["ZombieAlive"], nullptr, &destRect, 0.0, nullptr, flip);
        }else{
            SDL_RenderCopyEx(renderer, texture["withoutCone"], nullptr, &destRect, 0.0, nullptr, flip);
        }
        
        break;
        case DYING:
        destRect.x = currentPos.first+(grid_x_pos*GRID_WIDTH_PIXEL/GRID_LENGTH)-GRID_WIDTH_PIXEL;
        destRect.y = currentPos.second+(grid_y_pos*GRID_HEIGHT_PIXEL/GRID_LENGTH)-GRID_HEIGHT_PIXEL;
        SDL_RenderCopyEx(renderer, texture["ZombieBody"], nullptr, &destRect, 0.0, nullptr, flip);
        break;
        case DYING_ANIME:
        auto temp_rect=destRect;
        SDL_Point temp_point={destRect.w/2,destRect.h};
        SDL_RenderCopyEx(renderer, texture["ZombieBody"], nullptr, &destRect, angle, &temp_point, flip);
        
        break;


        }
}

void ConeHeadZombie::updateCollisionBox(){
    collisionRectMap["head"].x=destRect.x+17*zoom_rate;
    collisionRectMap["head"].y=destRect.y+6*zoom_rate;
    collisionRectMap["body"].x=destRect.x+13*zoom_rate;
    collisionRectMap["body"].y=destRect.y+14*zoom_rate;
}

bool ConeHeadZombie::is_collied_with_plant(const std::map<std::string,SDL_Rect>& destCollisionRect){
    if(zombieState==DYING||zombieState==FROZEN_ZOMBIE){
        return false;
    }
    for(const auto& pair_1:destCollisionRect){
        for(const auto&pair_2:collisionRectMap){
            if(destCollisionRect.empty()){
                continue;
            }
            if(SDL_HasIntersection(&pair_1.second, &pair_2.second)){
                zombieState=ATTACKING;
                return true;
            }
        }
    }
    zombieState=MOVING;
    return false;
}

void ConeHeadZombie::attack(std::unique_ptr<Plant>& target){
    if(feature.attack_coolDown<=15){
        feature.attack_coolDown++;
        return;
    }else{
        target->feature.health-=feature.damage;
        feature.attack_coolDown=0;
    }
    
}

void ConeHeadZombie::updateLivingCondition(){
    for(auto ite=protectiveList.begin();ite!=protectiveList.end();){
        if((*ite).health<0){
            protectiveList.erase(ite);
        } else{
            ++ite;
        }
    }
    

    if(feature.health<feature.critical_value&&feature.health>0){
        
        zombieState=DYING;
        isDying=true;
    }else if(feature.health<0){
        isDead=true;
        zombieState=DYING_ANIME;
    }
    if(zombieState==DYING){
        feature.health-=1;
    }
    
}

void ConeHeadZombie::init_zombie(const int row){
    zoom_rate=7,colPos=11,rowPos=row;
    grid_y_pos=rand()%(GRID_LENGTH/3);
    baseYPos=grid_y_pos;
    destRect.h=36*zoom_rate;
    destRect.w=36*zoom_rate;
    collisionRectMap["head"].h=10*zoom_rate;
    collisionRectMap["head"].w=10*zoom_rate;
    collisionRectMap["body"].h=14*zoom_rate;
    collisionRectMap["body"].w=7*zoom_rate;
}

void ConeHeadZombie::act(){
    updateLivingCondition();
    switch(zombieState){
        case MOVING:
        case DYING:
        move();
        updateCollisionBox();
        break;
        case ATTACKING:

        break;

        case DYING_ANIME:
        if(angle_change<4){
            angle_change+=0.5;
            switch(rand()%2){
                case 0:
                angle_change+=0.4;
                break;
                case 1:
                angle_change+=0.2;
            }
        }
        if(angle>=-90){
            angle-=angle_change;
        }else{
            zombieState=DEAD_ZOMBIE;
        }
        

        break;
        

             
    }
    
    
}