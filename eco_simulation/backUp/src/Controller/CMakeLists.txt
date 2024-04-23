# 从指定目录收集所有的源文件
file(GLOB CONTROLLERL_SOURCES "${PROJECT_SOURCE_DIR}/src/Controller/*.cpp")

# 创建一个库
add_library(Controller ${CONTROLLERL_SOURCES})

# 创建一个库
#add_library(Controller MainController.cpp)

# 使项目的 include 目录可用
target_include_directories(Controller PUBLIC ${PROJECT_SOURCE_DIR}/include/Controller)
target_include_directories(Model PUBLIC ${PROJECT_SOURCE_DIR}/include/Model)
target_include_directories(View PUBLIC ${PROJECT_SOURCE_DIR}/include/View)

