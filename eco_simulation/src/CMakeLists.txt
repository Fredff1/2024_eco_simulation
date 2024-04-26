# 添加子目录
add_subdirectory(Util)
add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

add_compile_options(-g)
# 指定生成的可执行文件和需要编译的源文件
add_executable(EcoSystem main.cpp)

# 查找 SDL2 库
find_library(SDL2_LIB NAME SDL2 PATHS ${SDL2_PATH}/lib NO_DEFAULT_PATH)
find_library(SDL2_MAIN_LIB NAME SDL2main PATHS ${SDL2_PATH}/lib NO_DEFAULT_PATH)
find_library(SDL2_TTF_LIB NAME SDL2_ttf PATHS ${SDL2_PATH}/lib NO_DEFAULT_PATH)
find_library(SDL2_IMAGE_LIB NAME SDL2_image PATHS ${SDL2_PATH}/lib NO_DEFAULT_PATH)

# 指定链接库的顺序
target_link_libraries(EcoSystem Util Model View Controller -lmingw32 ${SDL2_MAIN_LIB} ${SDL2_LIB} ${SDL2_TTF_LIB} ${SDL2_IMAGE_LIB})

# 如果使用 MinGW，可选择隐藏控制台窗口
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()