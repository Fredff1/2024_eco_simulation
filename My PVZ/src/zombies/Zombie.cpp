#include "Zombie.h"
#include "Game.h"

int Zombie::nextId=0;

void Zombie::loadZombieFeature(const ZombieType type){
    switch(type){
        case BasicZombieType:
        feature.attack_coolDown=0;
        feature.damage=30;
        feature.critical_value=70;
        feature.health=270;
        feature.speed=3.0+rand()%2;
        break;
        case ConeHeadZombieType:
        feature.attack_coolDown=0;
        feature.damage=30;
        feature.critical_value=70;
        feature.health=270;
        feature.speed=3.0+rand()%2;
        break;
    }   


}



//移动的逻辑
void BasicZombie::move(){
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


void BasicZombie::render_zombie(SDL_Renderer* renderer,const BackGroundState state) {
    // 更新destRect的位置
    SDL_RendererFlip flip = facing_left ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    auto currentPos=getPosFromGrid(rowPos,colPos,state);
    switch(zombieState){
        case MOVING: 
        case ATTACKING:
        destRect.x = currentPos.first+(grid_x_pos*GRID_WIDTH_PIXEL/GRID_LENGTH)-GRID_WIDTH_PIXEL;
        destRect.y = currentPos.second+(grid_y_pos*GRID_HEIGHT_PIXEL/GRID_LENGTH)-GRID_HEIGHT_PIXEL;
        // 渲染纹理
        SDL_RenderCopyEx(renderer, texture["ZombieAlive"], nullptr, &destRect, 0.0, nullptr, flip);
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

void BasicZombie::updateCollisionBox(){
    collisionRectMap["head"].x=destRect.x+17*zoom_rate;
    collisionRectMap["head"].y=destRect.y+6*zoom_rate;
    collisionRectMap["body"].x=destRect.x+13*zoom_rate;
    collisionRectMap["body"].y=destRect.y+14*zoom_rate;
}

bool BasicZombie::is_collied_with_plant(const std::map<std::string,SDL_Rect>& destCollisionRect){
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

void BasicZombie::attack(std::unique_ptr<Plant>& target){
    if(feature.attack_coolDown<=15){
        feature.attack_coolDown++;
        return;
    }else{
        target->feature.health-=feature.damage;
        feature.attack_coolDown=0;
    }
    
}

void ZombieFactory::setZombiePos(unique_ptr<Zombie>& target_zombie,const int row){
}

void BasicZombie::updateLivingCondition(){
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

void BasicZombie::init_zombie(const int row){
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

void BasicZombie::act(){
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

unique_ptr<Zombie> ZombieFactory::createZombie(const std::string& name,const int row){
    if(name=="BasicZombie"){
        unique_ptr target_ptr=std::make_unique<BasicZombie>();
        target_ptr->init_zombie(row);
        target_ptr->texture=_zombie_res.zombieTexture[BasicZombieType];
        
        return target_ptr;
    }else if(name=="ConeHeadZombie"){
        unique_ptr target_ptr=std::make_unique<ConeHeadZombie>();
        target_ptr->init_zombie(row);
        target_ptr->texture=_zombie_res.zombieTexture[ConeHeadZombieType];
        
        return target_ptr;
    }
    return nullptr;
}

