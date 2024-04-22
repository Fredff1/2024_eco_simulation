#include "View/MainView.hpp"
#include "GlobalEnum.hpp"
#include "Model/Consumer.hpp"

void MainFrame::initializeSDL(){
    
    try {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }

        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    

    
}

void MainFrame::render(){
        
    auto frameData=mainModel.getFrameData();
    int size=frameData->entitiesInAtlas.size();
    pre_size=size;
    SDL_SetRenderDrawColor(this->renderer,255,255,255,255);
    SDL_RenderClear(this->renderer);
    SDL_SetRenderDrawColor(renderer,0,0,200,255);
    SDL_RenderFillRect(renderer,&atlasView.getDestRectInUI());
    
    renderEntities(frameData);
    SDL_RenderPresent(this->renderer);
}

void MainFrame::renderEntities(const std::unique_ptr<FrameData>& frameData){
    for(auto entData:frameData->entitiesInAtlas){
        if(!intersect_(entData.rectInAtlas,mainModel.getQuadTreeAtlas().getVisibleRect())){
            continue;
        }
        auto tempRect=atlasView.convertRectToUI(entData.rectInAtlas);
        auto type=entData.type;
        switch(type){
            case PRODUCER_TYPE:
            SDL_SetRenderDrawColor(renderer,0,180,10,255);
            break;
            case CONSUMER_TYPE:
            SDL_SetRenderDrawColor(renderer,190,10,10,255);
            break;
            default:
            break;
        }
        SDL_IntersectRect(&atlasView.getDestRectInUI(),&tempRect,&tempRect);
        SDL_RenderFillRect(renderer,&tempRect);
    }
}