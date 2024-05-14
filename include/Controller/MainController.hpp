#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H
#include <thread>
#include <functional>

#include "Model/Consumer.hpp"
#include "View/MainView.hpp"
#include "ControllerData.hpp"
#include "GlobalEnum.hpp"
#include "GlobalStruct.hpp"
#include "Controller/CommandShell.hpp"
#include "Controller/ModelToControllerData.hpp"

#define DEBUG_MODE
class MainController{
private:
    MainFrame& frame;  // 控制的视图
    MainModel& model;  // 数据模型
    ShellCommand shellController;
    bool is_Running;
    SDL_Event event;
    ControllerData data;
    #ifdef DEBUG_MODE
    int currentFrameTime=200;
    #else 
    int currentFrameTime=60
    #endif

    void shellControllerThread(ShellCommand& shellCommand){
        shellCommand.startThread();
    }
public:
    MainController(MainFrame &mainFrame,MainModel &mainModel):frame(mainFrame),
    model(mainModel),shellController(frame,model){
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
                    data.event=END_GAME_CONTROLLER_ACTION;
                    model.addControllerData(data);
                    shellController.setRunning(false);
                    //exit(0);
                    break;
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                        int newWidth = event.window.data1;  // 新的宽度
                        int newHeight = event.window.data2; // 新的高度
                        
                        // 可以在这里调整渲染目标的大小
                        SDL_RenderSetLogicalSize(frame.getRenderer(), newWidth, newHeight);
                        frame.setAtlasMiddle(newWidth,newHeight);
                    }
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
        std::thread shellCommandThread(std::bind(&MainController::shellControllerThread, this,std::ref(shellController)));

        while(is_Running){
            handleEvents();
            auto frameStartTime = SDL_GetTicks();
            readModelToControllerData();
            data.visibleRect=frame.getAtlasView().getVisibleRect();
            model.addControllerData(data);
            is_Running=model.getRunning();

            auto frameEndTime = SDL_GetTicks(); // 获取循环结束的时间
            auto frameDuration = frameEndTime - frameStartTime; // 计算这一帧已消耗的时间
            frame.RunOneFrame();

            if (frameDuration < currentFrameTime) {
                SDL_Delay(currentFrameTime - frameDuration); // 如果完成得太快，等待剩余的时间
            }

        }
        frame.deleteAllResource();
        shellCommandThread.join();
    }

    bool isRunning() const{
        return is_Running;
    }

    void update(){
        
    }

    int getFPS(){
        return 1000/currentFrameTime;
    }

    void setFPS(int targetFPS){
        currentFrameTime=1000/targetFPS;
    }

    void readModelToControllerData(){
        auto& queue=model.getModelToControllerDataQueu();
        while(queue.empty()==false){
            std::unique_ptr<ModelToControllerData> data;
            queue.poll(data);
            actModelToControllerData(data);
            SDL_Delay(5);
        }
    }

    void actModelToControllerData(std::unique_ptr<ModelToControllerData>& data){

    }

};







#endif