# 从指定目录收集所有的源文件
file(GLOB MODEL_SOURCES "${PROJECT_SOURCE_DIR}/src/Model/*.cpp")

# 创建一个库
add_library(Model ${MODEL_SOURCES})

# 使项目的 include 目录可用

target_include_directories(Model PUBLIC ${PROJECT_SOURCE_DIR}/include/Model)


