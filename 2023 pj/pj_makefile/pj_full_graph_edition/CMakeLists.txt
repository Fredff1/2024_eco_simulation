cmake_minimum_required(VERSION 3.0)
project(SortMake)

# 设置 C++ 编译器
set(CMAKE_CXX_COMPILER "D:/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "D:/mingw64/bin/gcc.exe")

# 添加头文件搜索路径
include_directories(
    "D:/extra h for c cpp/SDL2-2.28.4/x86_64-w64-mingw32/include/SDL2"
    "D:/extra h for c cpp/SDL2_ttf-2.20.2/x86_64-w64-mingw32/include"
    "D:/extra h for c cpp/SDL2_image-2.6.3/x86_64-w64-mingw32/include"
    "include" # 您的项目的头文件目录
)

# 添加库文件搜索路径
link_directories(
    "D:/extra h for c cpp/SDL2-2.28.4/x86_64-w64-mingw32/lib"
    "D:/extra h for c cpp/SDL2_ttf-2.20.2/x86_64-w64-mingw32/lib"
    "D:/extra h for c cpp/SDL2_image-2.6.3/x86_64-w64-mingw32/lib"
)

# 将 src 目录和当前目录下的所有源文件添加到变量 SRC_SUB 和 SRC_CUR
aux_source_directory(src SRC_SUB)
aux_source_directory(. SRC_CUR)

# 创建可执行文件
add_executable(sort ${SRC_SUB} ${SRC_CUR})

# 链接 SDL2 和相关的库
target_link_libraries(sort mingw32 SDL2main SDL2 SDL2_ttf SDL2_image)



