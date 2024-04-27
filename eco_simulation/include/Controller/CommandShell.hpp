#ifndef COMMAND_SHELL_H
#define COMMAND_SHELL_H
#include "Model/Consumer.hpp"
#include "View/MainView.hpp"
#include "ControllerData.hpp"
#include "GlobalEnum.hpp"
#include "GlobalStruct.hpp"

class ShellCommand{
private:
    MainFrame& frame;  // 控制的视图
    MainModel& model;  // 数据模型
    bool isCommandShellAvailable=TRUST_E_NO_SIGNER_CERT;

    void handlePrintCommand(std::vector<std::string> commandArray){
        for(int i=1;i<commandArray.size();i++){
            switch(i){
                case 1:
                if(commandArray[i]=="window"){
                    auto rect=frame.getAtlasView().getVisibleRect();
                    std::cout<<rect.x<<" "<<rect.y<<std::endl;
                }
                break;
                case 2:

                break;
            }
        }
    }
public:
    ShellCommand(MainFrame &mainFrame,MainModel &mainModel):frame(mainFrame),model(mainModel){}

    void startThread(){
        while(isCommandShellAvailable){
            std::string command;
            std::cout<<"Enter your command"<<std::endl;
            std::cout<<"Your command: ";
            std::getline(std::cin,command);
            auto data=readCommand(command);
            model.addShellData(data);
            frame.addShellData(data);
        }
        
    }

    shellControllerData readCommand(const std::string command){
        shellControllerData data;
        auto commandArray=split(command," ");
        if(commandArray.size()<1){
            return data;
        }
        if(commandArray[0]=="pause"||commandArray[0]=="p"){
            data.event=PAUSE_MAIN_MODEL;
            std::cout<<"Simulation Paused"<<std::endl;
        }else if(commandArray[0]=="continue"||commandArray[0]=="c"){
            data.event=CONTINUE_MAIN_MODEL;
            std::cout<<"All pauses have been defused"<<std::endl;
        }else if(commandArray[0]=="print"){
            handlePrintCommand(commandArray);
        }else if(commandArray[0]=="quit"){
            data.event=QUIT_MAIN_MODEL;
            isCommandShellAvailable=false;
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
