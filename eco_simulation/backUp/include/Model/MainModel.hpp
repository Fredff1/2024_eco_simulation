#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H
#include "QuadTreeAtlas.hpp"
#include <queue>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <random>
#include <time.h>
#include "../Model/FrameDateBuffer.hpp"
#include "../Util/Observers.hpp"
#include "Util/ThreadSafeQueue.hpp"
#include "Controller/ControllerData.hpp"


class MainModel:public Subject<FrameData>{
private:
    QuadTreeAtlas quadTreeAtlas;
    ThreadSafeQueue<std::unique_ptr<FrameData>> dataQueue;
    ThreadSafeQueue<std::unique_ptr<ControllerData>> ControllerDataQueue;
    //std::queue<std::unique_ptr<FrameData>> dataQueue;
    FrameData newData;
    std::mutex queueMutex;  // Mutex to protect access to the queue
    std::condition_variable dataCond;  // Condition variable for notifying about available data
    std::mutex observersMutex;

    /*Convert current state into frameData to pass to View section*/
    void generateNewData();

   
    /* 一个帧里进行更新*/
    void readControllerData(){
        while(!ControllerDataQueue.empty()){
            std::unique_ptr<ControllerData> data;
            ControllerDataQueue.poll(data);
            actControllerCommand(data);
        }
    }

    /* change model state according to controller event 进行对应操作*/
    void actControllerCommand(std::unique_ptr<ControllerData>& data){
        switch(data->event){
            case NO_CONTROLLER_ACTION:
                
            break;
            default:

            break;
        }
        quadTreeAtlas.setVisibleRect(data->visibleRect);
    }

    /*add the generated data to the dataQueue*/
    void addFrameData(FrameData newDate){
        std::lock_guard<std::mutex> lock(queueMutex);
        dataQueue.push(std::make_unique<FrameData>(newData));
        dataCond.notify_one();
    }
public:
    MainModel(float Atlas_H,float Atlas_W):quadTreeAtlas(Atlas_H,Atlas_W){
      
    }

 /* Controller 类调用这个方法来添加信息*/
    void addControllerData(const ControllerData data){
        ControllerDataQueue.push(std::make_unique<ControllerData>(data));
    }

    QuadTreeAtlas& getQuadTreeAtlas(){
        return this->quadTreeAtlas;
    }
    /* The calculating thread*/
    void startThread(){
        while(true){
            if(dataQueue.size()<=100){
                update();
                SDL_Delay(2);
            }
        }
    }
    /* Update for one single frame*/
    void update(){
        readControllerData();
        generateNewData();
        addFrameData(newData);
        this->notifyObservers();
    }

   

    //get data from the queue
    std::unique_ptr<FrameData> getFrameData() {
        std::unique_lock<std::mutex> lock(queueMutex);
        dataCond.wait(lock, [this]{ return !dataQueue.empty(); });  // Wait until there is data
        std::unique_ptr<FrameData> data;
        dataQueue.poll(data);
        return data;
    }

    

    
    /*
    通知observer（即View）更新
    */
    void notifyObservers() override{
        for (Observer<FrameData>* observer : this->getObservers()) {
            observer->update(nullptr);
        }
    }

    
};










#endif