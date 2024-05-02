#include "View/MainView.hpp"
#include "Model/Consumer.hpp"
#include "Model/MainModel.hpp"
#include "Util/RandomUtil.hpp"
#include "Controller/CommandShell.hpp"
#include "Util/ThreadSafeCout.hpp"
#include <thread>
#include "Controller/MainController.hpp"
//#include <windows.h>  // 引入Windows API
// Model更新线程函数
void modelThread(MainModel& model) {
    // 设置模型线程为较低优先级
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    model.startThread();
}

void commandShellThread(ShellCommand& shellCommand){
    shellCommand.startThread();
}

int main(int argc, char* argv[]) {
    MainModel mainModel(ATLAS_FULL_HEIGHT, ATLAS_FULL_WIDTH);
    MainFrame mainFrame(mainModel);

    ShellCommand shellController(mainFrame,mainModel);
    MainController mainController(mainFrame,mainModel);
    // 创建模型处理线程
    std::thread modelThreadHandle(modelThread, std::ref(mainModel));
    //std::thread shellThreadHandle(commandShellThread,std::ref(shellController));
    // 在主线程中运行视图渲染逻辑
    // 设置主线程的优先级更高
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    mainController.mainThread();
    

    // 等待模型线程结束
    modelThreadHandle.join();
    
    //shellThreadHandle.join();
    return 0;
}