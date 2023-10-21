cmake_minimum_required(VERSION 3.10)
project(new_graph VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(main main.cpp)


target_link_libraries(main mingw32)
target_link_libraries(main SDL2main)
target_link_libraries(main SDL2) 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
