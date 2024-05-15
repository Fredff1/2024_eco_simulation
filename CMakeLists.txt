cmake_minimum_required(VERSION 3.14)
project(EcoSystem)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置 SDL2 的路径（确保这个路径是正确的）
set(SDL2_PATH "${PROJECT_SOURCE_DIR}/SDL2-2.28.4/x86_64-w64-mingw32")

# 设置编译器标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_BUILD_TYPE Debug)  # 确保以 Debug 模式编译

add_compile_options(-O0)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/Util)
include_directories(${SDL2_PATH}/include)

# 设置链接器目录
link_directories(${SDL2_PATH}/lib)

# 添加 src 目录
add_subdirectory(src)

# 定义资源文件
file(GLOB MY_ASSETS "${PROJECT_SOURCE_DIR}/Assets/*")

# 设置资源复制的目标目录
set(ASSETS_DIR "${CMAKE_BINARY_DIR}/src/Assets")

# 创建资源目录
file(MAKE_DIRECTORY ${ASSETS_DIR})

# 复制资源文件到目标目录
foreach(asset ${MY_ASSETS})
    get_filename_component(filename ${asset} NAME)
    set(output "${ASSETS_DIR}/${filename}")

    add_custom_command(
        OUTPUT ${output}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${asset} ${output}
        DEPENDS ${asset}
        COMMENT "Copying ${filename} to assets directory"
    )
    list(APPEND ASSETS_COPIED ${output})
endforeach()

# 创建一个自定义目标以确保资源被复制
add_custom_target(copy_assets ALL DEPENDS ${ASSETS_COPIED})

# 假设你的可执行文件目标是 'EcoSystemApp'
# 确保可执行文件目标依赖于 'copy_assets' 目标
add_dependencies(EcoSystem copy_assets)

# 链接 SDL2 库
target_link_libraries(EcoSystem SDL2 SDL2main)
