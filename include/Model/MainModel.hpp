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
#include "Controller/ShellControllerData.hpp"
#include "Controller/ModelToControllerData.hpp"


class MainModel:public Subject<FrameData>{
private:
    QuadTreeAtlas quadTreeAtlas;
    ThreadSafeQueue<std::unique_ptr<FrameData>> dataQueue;
    ThreadSafeQueue<std::unique_ptr<ControllerData>> ControllerDataQueue;
    ThreadSafeQueue<std::unique_ptr<shellControllerData>> shellDataQueue;
    ThreadSafeQueue<std::unique_ptr<ModelToControllerData>> modelToCOntrollerDataQueue;
    //std::queue<std::unique_ptr<FrameData>> dataQueue;
    FrameData newData;
    std::mutex queueMutex;  // Mutex to protect access to the queue
    std::condition_variable dataCond;  // Condition variable for notifying about available data
    std::mutex observersMutex;

    bool is_running=true;
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

    void readShellControllerData(){
        while(!shellDataQueue.empty()){
            std::unique_ptr<shellControllerData> data;
            shellDataQueue.poll(data);
            actShellControllerData(data);
        }
    }

    void addModelToControllerData(const ModelToControllerData& data){
        modelToCOntrollerDataQueue.push(std::make_unique<ModelToControllerData>(data));
    }

    /* change model state according to controller event 进行对应操作*/
    void actControllerCommand(std::unique_ptr<ControllerData>& data){
        switch(data->event){
            case NO_CONTROLLER_ACTION:
                
            break;
            case END_GAME_CONTROLLER_ACTION:
            is_running=false;
            break;
            default:

            break;
        }
        quadTreeAtlas.setVisibleRect(data->visibleRect);
    }

    void actShellControllerData(std::unique_ptr<shellControllerData>& data){
        switch(data->event){
            case NO_SHELL_EVENT:

            break;
            case PAUSE_MAIN_MODEL:
            /* here it has no usage ,the cmd is for render*/
            break;
            case CONTINUE_MAIN_MODEL:
            /* here it has no usage ,the cmd is for render*/
            break;
            case QUIT_MAIN_MODEL:
            is_running=false;
            break;
            case INSERT_MAIN_MODEL:
            if(auto iData=data->getData<cmdInsertData>()){
                auto insertType=iData->type;
                if(insertType==CMD_WATER_CURRENT){
                    auto targetFeature=iData->getData<WaterFlowFeature>();
                    WaterFlow newWaterFLow=WaterFlow(*targetFeature,0);
                    quadTreeAtlas.insertWaterFlow(newWaterFLow);
                    auto rect=newWaterFLow.feature.targetArea;
                    ThreadSafeCout()<<"Successfully insert waterFlow to area at ("<<rect.x<<","<<rect.y<<")";
                    ThreadSafeCout()<<"The size of waterFlow: "<<rect.h<<"x"<<rect.w<<"\n";
                }else if(insertType==CMD_WATER_VORTEX){
                    auto targetFeature=iData->getData<WaterFlowFeature>();
                    WaterFlow newWaterFLow=WaterFlow(*targetFeature,0);
                    quadTreeAtlas.insertWaterFlow(newWaterFLow);
                    auto rect=newWaterFLow.feature.targetArea;
                    ThreadSafeCout()<<"Successfully insert waterFlow to area at ("<<rect.x<<","<<rect.y<<")";
                    ThreadSafeCout()<<"The size of waterFlow: "<<rect.h<<"x"<<rect.w<<"\n";
                }else if(insertType==CMD_CONSUMER_ENTITY){
                    auto targetFeature=iData->getData<EntityFeature>();
                    auto ent=quadTreeAtlas.getEntityFactory().createEntity(CONSUMER_TYPE,quadTreeAtlas.getRoot(),*targetFeature);
                    quadTreeAtlas.insertEntity(quadTreeAtlas.getRoot(),ent);
                    ThreadSafeCout()<<"Successfully insert consumer to given position\n";
                }else if(insertType==CMD_PRODUCER_ENTITY){
                    auto targetFeature=iData->getData<EntityFeature>();
                    auto ent=quadTreeAtlas.getEntityFactory().createEntity(PRODUCER_TYPE,quadTreeAtlas.getRoot(),*targetFeature);
                    quadTreeAtlas.insertEntity(quadTreeAtlas.getRoot(),ent);
                    ThreadSafeCout()<<"Successfully insert producer to given position\n";
                }
            }else{
                ThreadSafeCout()<<"Error when handling insert data\n";
            }
            
            break;
            case INIT_MAIN_MODEL:
            if(auto iData=data->getData<cmdInitData>()){
                quadTreeAtlas.initEntities(iData->consumerNum,iData->producerNum,0);
                int count=0;
                while(count<20){
                    update();
                    SDL_Delay(1);
                    count++;
                }
                ThreadSafeCout()<<"Successfully init entities\n";
            }
            break;
        }
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

    void addShellData(const shellControllerData data){
        shellDataQueue.push(std::make_unique<shellControllerData>(data));
    }

    QuadTreeAtlas& getQuadTreeAtlas(){
        return this->quadTreeAtlas;
    }
    /* The calculating thread*/
    void startThread(){
        while(is_running){
            if(dataQueue.size()<=5){
                update();
                SDL_Delay(1);
            }
        }
    }
    /* Update for one single frame*/
    void update(){
        readControllerData();
        readShellControllerData();
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

    
    void updateRunning(bool input){
        is_running=input;
    }

    bool getRunning(){
        return is_running;
    }
    
    /*
    通知observer（即View）更新
    */
    void notifyObservers() override{
        for (Observer<FrameData>* observer : this->getObservers()) {
            observer->update(nullptr);
        }
    }

    ThreadSafeQueue<std::unique_ptr<ModelToControllerData>>& getModelToControllerDataQueu(){
        return this->modelToCOntrollerDataQueue;
    }

    
};










#endif