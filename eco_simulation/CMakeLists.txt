cmake_minimum_required(VERSION 3.14)
project(EcoSystem)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置 SDL2 的路径（确保这个路径是正确的）
set(SDL2_PATH "${PROJECT_SOURCE_DIR}/SDL2-2.28.4/x86_64-w64-mingw32")
add_compile_options(-O0)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/Util)
include_directories(${SDL2_PATH}/include)

# 设置链接器目录（可能不需要，视具体情况而定）
link_directories(${SDL2_PATH}/lib)

# 添加 src 目录
add_subdirectory(src)