# 从指定目录收集所有的源文件
file(GLOB VIEW_SOURCES "${PROJECT_SOURCE_DIR}/src/View/*.cpp")

# 创建一个库
add_library(View ${VIEW_SOURCES})

# 使项目的 include 目录可用

target_include_directories(Model PUBLIC ${PROJECT_SOURCE_DIR}/include/Model)
target_include_directories(View PUBLIC ${PROJECT_SOURCE_DIR}/include/View)

