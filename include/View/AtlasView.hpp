#ifndef ATLAS_VIEW_H
#define ATLAS_VIEW_H

#include "Model/QuadTreeAtlas.hpp"
#include "GlobalStruct.hpp"
#include <vector>
#include <SDL2/SDL.h>
#include "Util/RandomUtil.hpp"
#include <list>

class AtlasView{
private:
    QuadTreeAtlas& quadTreeAtlas;
    Point positionInWindow;
    SDL_Rect visibleRect;
    SDL_Rect destRectInUI;
    std::list<std::shared_ptr<Entity>> visibleEntities;
    
public:
    AtlasView(QuadTreeAtlas& quadTree):quadTreeAtlas(quadTree),positionInWindow(100,100){

    }

    ~AtlasView(){}

    void setVisibleRect(SDL_Rect targetRect){
        RandomUtil::getRandomDouble(1,2);
        visibleRect=targetRect;
        quadTreeAtlas.setVisibleRect(targetRect);
    }

    void setDestRectInUI(SDL_Rect targetRect){
        destRectInUI=targetRect;
    }

    SDL_Rect& getVisibleRect(){
        return visibleRect;
    }

    SDL_Rect& getDestRectInUI(){
        return destRectInUI;
    }

    void setPosInWindow(float x,float y){
        positionInWindow.x=x;
        positionInWindow.y=y;
    }

    Point getPosInWindow(){

        return positionInWindow;
    }

   

    SDL_Rect convertRectToUI(const SDL_Rect& rectInAtlas){
        SDL_Rect result;
        result.h=rectInAtlas.h;
        result.w=rectInAtlas.w;
        result.x=rectInAtlas.x-visibleRect.x+destRectInUI.x;
        result.y=rectInAtlas.y-visibleRect.y+destRectInUI.y;
        return result;
    }
    /* 先除去不符合要求的实体，再渲染*/
    void renderRemoveENtities(const SDL_Rect& viewport,SDL_Renderer *renderer){
        auto it = visibleEntities.begin();
        while (it != visibleEntities.end()) {
            if (isEntityInViewport(*it, viewport)) {
                auto targetRect=convertRectToUI((*it)->getRectInAtlas());
                (*it)->render(renderer,targetRect);  // 渲染实体
                ++it;
            } else {
                it = visibleEntities.erase(it);  // 删除不在视口内的实体
            }
        }
    }

    bool isEntityInViewport(const std::shared_ptr<Entity>& entity, const SDL_Rect& viewport) {
        SDL_Rect entRect =entity->getRectInAtlas();
        // 简单的矩形相交检查
        return (entRect.x < viewport.x + viewport.w) && (entRect.x + entRect.w > viewport.x) &&
            (entRect.y < viewport.y + viewport.h) && (entRect.y + entRect.h > viewport.y);
    }

    /* 渲染地图背景*/
    void renderAtlas(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
        SDL_RenderFillRect(renderer,&destRectInUI);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
    }

    

    /* 每一帧刷新状态并渲染*/
    void updateRender(SDL_Renderer* renderer){
        // quadTreeAtlas.queryUpdateEntitiesInArea(quadTreeAtlas.getRoot(),visibleRect,visibleEntities);
        // renderAtlas(renderer);
        // renderRemoveENtities(visibleRect,renderer);
    }



};











#endif