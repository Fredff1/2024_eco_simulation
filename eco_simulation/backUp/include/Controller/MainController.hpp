#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H
#include "Model/Consumer.hpp"
#include "View/MainView.hpp"
#include "ControllerData.hpp"
#include "GlobalEnum.hpp"
#include "GlobalStruct.hpp"

class MainController{
private:
    MainFrame& frame;  // 控制的视图
    MainModel& model;  // 数据模型
    bool is_Running;
    SDL_Event event;
    ControllerData data;
public:
    MainController(MainFrame &mainFrame,MainModel &mainModel):frame(mainFrame),
    model(mainModel){
        is_Running=true;
    }

    ~MainController(){
        
    }

    void handleEvents(){
        ControllerData data;
        while(SDL_PollEvent(&event)!=0){
            switch(event.type){
                case SDL_QUIT:
                    is_Running = false;
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            // 向左移动摄像头
                            //view.moveCamera(-10, 0);
                            frame.moveCamera(-50,0);
                            break;
                        case SDLK_RIGHT:
                            // 向右移动摄像头
                            //view.moveCamera(10, 0);
                            frame.moveCamera(50,0);
                            break;
                        case SDLK_UP:
                            // 向上移动摄像头
                            //view.moveCamera(0, -10);
                            frame.moveCamera(0,-50);
                            break;
                        case SDLK_DOWN:
                            // 向下移动摄像头
                            //view.moveCamera(0, 10);
                            frame.moveCamera(0,50);
                            break;
                    }
                    break;
            }
            
        }
    }

    void mainThread(){
        const int targetFrameTime = 75; // 目标帧时间为75毫秒，约等于14FPS
        while(is_Running){
            handleEvents();
            auto frameStartTime = SDL_GetTicks();

            data.visibleRect=frame.getAtlasView().getVisibleRect();
            model.addControllerData(data);

            auto frameEndTime = SDL_GetTicks(); // 获取循环结束的时间
            auto frameDuration = frameEndTime - frameStartTime; // 计算这一帧已消耗的时间
            frame.RunOneFrame();

            if (frameDuration < targetFrameTime) {
                SDL_Delay(targetFrameTime - frameDuration); // 如果完成得太快，等待剩余的时间
            }

        }
    }

    bool isRunning() const{
        return is_Running;
    }

    void update(){
        
    }

};







#endif