#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

#include <variant>
#include <iostream>
#include "Model/WaterFlow.hpp"
#include "GlobalEnum.hpp"
#include "Model/Entity.hpp"
#include "ThreadSafeCout.hpp"

enum shellCmdEvent{
    NO_SHELL_EVENT,
    PAUSE_MAIN_MODEL,
    CONTINUE_MAIN_MODEL,
    QUIT_MAIN_MODEL,
    INSERT_MAIN_MODEL,
    INIT_MAIN_MODEL,
};

enum shellInsertType{
    CMD_NO_INSERT_TYPE,
    CMD_WATER_CURRENT,
    CMD_WATER_VORTEX,
    CMD_CONSUMER_ENTITY,
    CMD_PRODUCER_ENTITY,
};

using InsertedFeature=std::variant<EntityFeature,WaterFlowFeature>;

struct cmdInsertData{
    shellInsertType type=CMD_NO_INSERT_TYPE;
    InsertedFeature feature;

    template <typename T>
    T* getData() {
        return std::get_if<T>(&feature);
    }

    cmdInsertData(shellInsertType type):type(type){}
   
};

struct cmdDefaultData{
    bool flag=true;
};

struct cmdViewData{

};

struct cmdGlobalChangeData{

};

struct cmdInitData{
    int consumerNum;
    int producerNum;
    void setData(int producerN,int consumerN){
        consumerNum=consumerN;
        producerNum=producerN;
    }
};

using CmdVariant = std::variant<cmdInsertData, cmdDefaultData, cmdViewData, cmdGlobalChangeData,cmdInitData>;

struct shellControllerData{
    shellCmdEvent event;
    CmdVariant dataType;
    shellControllerData():event(NO_SHELL_EVENT),dataType(cmdDefaultData()){        
    }
    void processData() {
        std::visit([this](auto&& arg) {
            //代表arg的去除引用、常量和易失性限定符后的基础类型
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, cmdDefaultData>) {
                ThreadSafeCout()<< "Processing cmdDefaultData with flag: " << arg.flag << std::endl;
            } else if constexpr (std::is_same_v<T, cmdInsertData>) {
                ThreadSafeCout()<< "Processing cmdInsertData" << std::endl;
            } else if constexpr (std::is_same_v<T, cmdViewData>) {
                ThreadSafeCout() << "Processing cmdViewData" << std::endl;
            } else if constexpr (std::is_same_v<T, cmdGlobalChangeData>) {
                ThreadSafeCout() << "Processing cmdGlobalChangeData" << std::endl;
            }
        }, dataType);
    }

    template <typename T>
    T* getData() {
        return std::get_if<T>(&dataType);
    }

    
};

#endif 
