#include "View/MainView.hpp"
#include "GlobalEnum.hpp"
#include "Model/Consumer.hpp"
#include "ThreadSafeCout.hpp"
#include <stdio.h>
void MainFrame::initializeSDL(){
    
    try {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }

        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
        }

       
         this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        auto err=std::string(SDL_GetError());
        //std::cout<<std::string(SDL_GetError())<<std::endl;
        if(renderer==nullptr){
            throw std::runtime_error("Renderer failed to init" + std::string(SDL_GetError()));
        }

           

    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        //SDL_DestroyWindow(window);
        //SDL_Quit();
        //exit(1);
    }
    this->textureManager.setRenderer(this->renderer);
    

    
}

void MainFrame::render(){
    
    if(!isRenderPaused){
        auto frameData=mainModel.getFrameData();
        int size=frameData->entitiesInAtlas.size();
        pre_size=size;
        
        readShellControllerData();
        SDL_SetRenderDrawColor(this->renderer,255,255,255,255);
        SDL_RenderClear(this->renderer);
        textureManager.drawTexture("backGround",&atlasView.getVisibleRect(),&atlasView.getDestRectInUI());
        renderEntities(frameData);
        SDL_RenderPresent(this->renderer);
        lastData=std::move(frameData);
    }else if(lastData&&isRenderPaused){
        auto& frameData=lastData;
        readShellControllerData();
        SDL_SetRenderDrawColor(this->renderer,255,255,255,255);
        SDL_RenderClear(this->renderer);

        textureManager.drawTexture("backGround",&atlasView.getVisibleRect(),&atlasView.getDestRectInUI());
        renderEntities(frameData);
        SDL_RenderPresent(this->renderer);
    }
    
    
    
    
}

void MainFrame::renderEntities(const std::unique_ptr<FrameData>& frameData){
    for(auto entData:frameData->entitiesInAtlas){
        if(!intersect_(entData.rectInAtlas,mainModel.getQuadTreeAtlas().getVisibleRect())){
            continue;
        }
        if(entData.isAlive==false){
            continue;
        }

      

        auto fullRect=atlasView.convertRectToUI(entData.rectInAtlas);
        auto type=entData.type;
        //auto entColor=entData.color;
        auto angle=entData.movingAngle;
        //SDL_SetRenderDrawColor(renderer,entColor.r,entColor.g,entColor.b,entColor.a);
        
        
        SDL_IntersectRect(&atlasView.getDestRectInUI(),&fullRect,&fullRect);
        if(fullRect.y<atlasView.getDestRectInUI().y){
            ThreadSafeCout()<<"error for render position\n";
        }
        if(fullRect.h>5000||fullRect.w>5000){
            ThreadSafeCout()<<"Error target for entity id"<<entData.id<<"entity type"<<entData.type<<std::endl;
            continue;
        }
        //SDL_RenderFillRect(renderer,&tempRect);
        switch(type){
            case PRODUCER_TYPE:
            textureManager.drawTexture("Producer_Normal.png",&fullRect);
            break;
            case CONSUMER_TYPE:
            textureManager.drawTexture("Consumer_Normal.png",&fullRect,angle);
            break;
            default:
            break;
        }
        
    }
}