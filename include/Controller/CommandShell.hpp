#ifndef COMMAND_SHELL_H
#define COMMAND_SHELL_H
#include "Model/Consumer.hpp"
#include "Model/Producer.hpp"
#include "View/MainView.hpp"
#include "ControllerData.hpp"
#include "GlobalEnum.hpp"
#include "GlobalStruct.hpp"

#include <atomic>
#include <future>

class ShellCommand{
private:
    MainFrame& frame;  // 控制的视图
    MainModel& model;  // 数据模型
    std::atomic<bool> isCommandShellAvailable{true};


    void startSimulation(){
        ThreadSafeCout()<<"Welcome to ecosystem simulation ver 1.0!\n";
        ThreadSafeCout()<<"Now we need to  init the atlas\n";
        shellControllerData data;
        handleInitCommand(data);
        frame.addShellData(data);
        model.addShellData(data);

        shellControllerData pauseData;
        pauseData.event=PAUSE_MAIN_MODEL;
        frame.addShellData(pauseData);
        SDL_Delay(2000);
        shellControllerData continueData;
        continueData.event=CONTINUE_MAIN_MODEL;
        frame.addShellData(continueData);
    }

    void handleInitCommand(shellControllerData& data){
        int consumerNum=0,producerNum=0;
        
        ThreadSafeCout()<<"Please enter the number of producers\n";
        if(std::cin>>producerNum){
            ThreadSafeCout()<<"Successfully set producer number to "<<producerNum<<"\n";
        }else{
            ThreadSafeCout()<<"Invalid input!Automaticaly set producer number to 500\n";
        }
        ThreadSafeCout()<<"Please enter the number of consumers\n";
        if(std::cin>>consumerNum){
            ThreadSafeCout()<<"Successfully set consumer number to "<<consumerNum<<"\n";
        }else{
            ThreadSafeCout()<<"Invalid input!Automaticaly set consumer number to 500\n";
        }
        data.event=INIT_MAIN_MODEL;
        /* init the data to be send to model*/
        data.dataType=cmdInitData();
        data.getData<cmdInitData>()->setData(producerNum,consumerNum);
        ThreadSafeCout()<<"Please wait to insert all the entities";
    }

    void handlePrintCommand(std::vector<std::string> commandArray){
        for(int i=1;i<commandArray.size();i++){
            switch(i){
                case 1:
                if(commandArray[i]=="window"){
                    auto rect=frame.getAtlasView().getVisibleRect();
                    ThreadSafeCout()<<rect.x<<" "<<rect.y<<std::endl;
                }
                break;
                case 2:

                break;
            }
        }
    }
    //a right command example: insert consumer 100 100 (Optional:size h w) 
    //(Optional life)
    shellInsertType convertStringToType(const std::string& input){
        if(input=="Consumer"||input=="consumer"){
            return CMD_CONSUMER_ENTITY;
        }else if(input=="Producer"||input=="producer"){
            return CMD_PRODUCER_ENTITY;
        }else if(input=="Current"||input=="current"||input=="Water"||input=="water"){
            return CMD_WATER_CURRENT;
        }else if(input=="Vortex"||input=="vortex"){
            return CMD_WATER_VORTEX;
        }else{
            return CMD_NO_INSERT_TYPE;
        }
    }

    void readTwoInt(int& x,int& y){
        while (true) {
            if (std::cin >> x >> y) {
                // 如果成功读取了x和y
                break;  // 退出循环
            } else {
                // 如果输入错误（例如输入了非整数）
                ThreadSafeCout() << "\nInvalid input. Please enter integers.\n";
                std::cin.clear();  // 清除错误状态
                // 忽略直到行尾的所有字符
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    void readWaterLife(int& life){
        if(std::cin>>life){
                ThreadSafeCout()<<"\nSuccessfully set life of the water current";
        }else{
            ThreadSafeCout()<<"\nFailed to read.The life will be set default";
        }
    }

    void handleInsertCommand(std::vector<std::string>& commandArray,shellControllerData& data){
        std::string input;
        std::cout<<"Please enter the specific type\n";
        std::cin>>input;
        shellInsertType targetType=convertStringToType(input);
        int x=0,y=0,h=0,w=0,life=0;
        int flowSpeed=0,flowAngle=0;
        /* Set data to selected type*/
        data.dataType=cmdInsertData(targetType);
        data.event=INSERT_MAIN_MODEL;
        switch(targetType){
            case CMD_WATER_CURRENT:
            ThreadSafeCout() << "Enter position (x y): ";
            readTwoInt(x,y);
            ThreadSafeCout()<< "\nEnter size :";
            readTwoInt(h,w);
            ThreadSafeCout()<<"\nEnter lasting time of the current,if not valid input,will set default lasting time\n";
            readWaterLife(life);
            ThreadSafeCout()<<"Enter the flow speed(0-any) and the angle of the straight current(0~360)\n";
            readTwoInt(flowSpeed,flowAngle);
            if(flowSpeed<0||flowSpeed>50){
                ThreadSafeCout()<<"Warning: The input speed is dangerous,automatically set default speed\n";
                flowSpeed=5;
            }
            if(flowAngle<0||flowAngle>360){
                ThreadSafeCout()<<"Warning: The input angle is dangerous,automatically set safe\n";
                flowAngle=std::abs(flowAngle)%360;
            }
            data.getData<cmdInsertData>()->feature=WaterFlowFeature(initRect(x,y,h,w),flowAngle,flowSpeed,life,0);
            //data.getData<cmdInsertData>()->feature=WaterFlowFeature()
            break;
            case CMD_WATER_VORTEX:
            ThreadSafeCout() << "\nEnter position (x y): ";
            readTwoInt(x,y);
            ThreadSafeCout()<< "\nEnter size :";
            readTwoInt(h,w);
            ThreadSafeCout()<<"\nEnter lasting time of the current,if not valid input,will set default lasting time\n";
            readWaterLife(life);
            //data.dataTYpe=cmdInsertData(targetType,x,y,h,w,life);
            ThreadSafeCout()<<"Enter the flow speed of the vortex\n";
            std::cin>>flowSpeed;
            if(flowSpeed<0||flowSpeed>50){
                ThreadSafeCout()<<"Warning: The input speed is dangerous,automatically set default speed\n";
                flowSpeed=5;
            }
            data.getData<cmdInsertData>()->feature=WaterFlowFeature(initRect(x,y,h,w),flowSpeed,life,0);
            break;
            case CMD_CONSUMER_ENTITY:
            handleCreateEntity(CONSUMER_TYPE,data);
            break;
            case CMD_PRODUCER_ENTITY:
            handleCreateEntity(PRODUCER_TYPE,data);
            break;
            case CMD_NO_INSERT_TYPE:
            std::cout<<"Invalid type input\n";
            break;
        }
    }

    void handleCreateEntity(EntityType type,shellControllerData& data){
        int x,y,h;
        ThreadSafeCout() << "\nEnter position (x y): ";
        readTwoInt(x,y);
        ThreadSafeCout()<<"Enter the max size of the Consumer,if the input is not valid,will use default size\n";
        if(std::cin>>h){
            ThreadSafeCout()<<"Successfully read size as "<<h<<"\n";
        }else{
            h=35;
        }
        EntityFeatureInitMsg newMsg(false,false,1);
        if(type==CONSUMER_TYPE){
            newMsg=EntityFeatureInitMsg(true,true,1);
        }else if (type==PRODUCER_TYPE){
            newMsg=EntityFeatureInitMsg(true,true,2.5);
        }
        auto newFeature=EntityFeature(newMsg);
        newFeature.readPosData(x,y,h);
        newFeature.type=type;
        data.getData<cmdInsertData>()->feature=newFeature;
    }

public:
    ShellCommand(MainFrame &mainFrame,MainModel &mainModel):frame(mainFrame),model(mainModel){}

    void startThread(){
        startSimulation();

        while(isCommandShellAvailable.load()){
            std::string command;
            std::cin.clear();
            ThreadSafeCout()<<"Enter your command"<<std::endl;
            std::getline(std::cin,command);
            auto data=readCommand(command);
            model.addShellData(data);
            frame.addShellData(data);
            SDL_Delay(100);
        }
        
    }
    


    void setRunning(bool flag){
        isCommandShellAvailable.store(flag, std::memory_order_relaxed);
        ThreadSafeCout()<<std::endl;
        ThreadSafeCout()<<"Waiting to end the last thread"<<std::endl;
        ThreadSafeCout()<<"Enter any content to quit the simulation\n";
        SDL_Delay(100);
        //exit(0);
    }

    shellControllerData readCommand(const std::string command){
        shellControllerData data;
        auto commandArray=split(command," ");
        if(commandArray.size()<1){
            return data;
        }
        if(commandArray[0]=="pause"||commandArray[0]=="p"){
            data.event=PAUSE_MAIN_MODEL;
            ThreadSafeCout()<<"Simulation Paused"<<std::endl;
        }else if(commandArray[0]=="continue"||commandArray[0]=="c"){
            data.event=CONTINUE_MAIN_MODEL;
            ThreadSafeCout()<<"All pauses have been defused"<<std::endl;
        }else if(commandArray[0]=="print"){
            handlePrintCommand(commandArray);
        }else if(commandArray[0]=="quit"){
            data.event=QUIT_MAIN_MODEL;
            isCommandShellAvailable.store(false, std::memory_order_relaxed);
        }else if(commandArray[0]=="insert"){
            handleInsertCommand(commandArray,data);
        }else if(commandArray[0]=="init"){
            handleInitCommand(data);
        }
        
        return data;
    }

    std::vector<std::string> split(const std::string& s,const std::string& delimiters) {
         std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = s.find_first_of(delimiters);

        while (end != std::string::npos) {
            if (end != start) {
                tokens.push_back(s.substr(start, end - start));
            }
            start = end + 1;
            end = s.find_first_of(delimiters, start);
        }

        if (start != s.length()) {  // 添加最后一个token
            tokens.push_back(s.substr(start));
        }

        return tokens;
    }
};


#endif 
