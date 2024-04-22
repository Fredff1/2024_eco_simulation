#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <atomic>   
#include "Model/MainModel.hpp"
#include "Model/FrameDateBuffer.hpp"
#include "GlobalEnum.hpp"
#include "Observers.hpp"
#include "AtlasView.hpp"
#include <windows.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960


#define FPS 1

//GUI的主组件
class MainFrame:public Observer<FrameData>{
private:
    std::atomic<bool> isNextFrameReady;
    std::queue<FrameData> nextDataToRender;
    //FrameData nextDataToRender;
    MainModel& mainModel;

    AtlasView atlasView;
    int pre_size=10;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Surface* screenSurface;

    SDL_Rect initRect(int x,int y,int height,int width){
        SDL_Rect result;
        result.x=x,result.y=y,result.h=height,result.w=width;
        return result;
    }

    /*init SDL window renderer surface*/
    void initializeSDL();

    /* render the entities in the frameData's entity list*/
    void renderEntities(const std::unique_ptr<FrameData>& frameData);

   
public:
    MainFrame(MainModel& m):mainModel(m),atlasView(m.getQuadTreeAtlas()){
        mainModel.attach(this);
        initializeSDL();
        /* 地图的位置*/
        atlasView.setDestRectInUI(initRect(150,200,750,750));
        atlasView.setVisibleRect(initRect(500,500,750,750));
    }

    /*
    frame 的生命周期短于Model来保证不会出现内存问题
    */
    ~MainFrame(){
        mainModel.detach(this);
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        SDL_Quit();
    }

    AtlasView& getAtlasView(){
        return atlasView;
    }

    /* start the render thread*/
    void RunOneFrame(){  
        if (isNextFrameReady.load(std::memory_order_acquire)&&nextDataToRender.size()<=100) {
            //auto data = mainModel.getFrameData();  // 从模型获取帧数据
            
            render();
            
                
        } else {
            isNextFrameReady.store(false, std::memory_order_release);
        }
    }

    //跟新下一帧的数据
    void update(const FrameData* data) override {
        isNextFrameReady.store(true, std::memory_order_release);
    }

    //渲染的主函数
    void render();

    void moveCamera(int x,int y){
        auto targetRect=atlasView.getVisibleRect();
       
        
        
        
        if(targetRect.x+x<0){
            targetRect.x=0;
        }else if(targetRect.x+x>4096){
            targetRect.x=4096;
        }else{
            targetRect.x=(targetRect.x+x+4096)%4096;
        }
        
        if(targetRect.y+y<0){
            targetRect.y=0;
        }else if(targetRect.y+y>4096){
            targetRect.y=4096;
        }else{
            targetRect.y=(targetRect.y+y+4096)%4096;
        }
        atlasView.setVisibleRect(targetRect);
    }
};










#endif