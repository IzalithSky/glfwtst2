cmake_minimum_required(VERSION 3.6)

project(glfwtst2)

set(GLFW3_LIB "C:/software/glfw/glfw-3.3.bin.WIN32/lib-mingw/glfw3.dll")
set(GLFW3_INCLUDE "C:/software/glfw/glfw-3.3.bin.WIN32/include")

add_executable(tst
    main.cpp
    glad.c
) 

include_directories(${GLFW3_INCLUDE})

target_link_libraries(tst ${GLFW3_LIB})
