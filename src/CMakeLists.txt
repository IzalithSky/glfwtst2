cmake_minimum_required(VERSION 3.6)

project(glfwtst2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

configure_file(
	"${PROJECT_SOURCE_DIR}/vertex_shader.glsl.in"
	"${PROJECT_BINARY_DIR}/vertex_shader.glsl"
    COPYONLY)

configure_file(
	"${PROJECT_SOURCE_DIR}/fragment_shader.glsl.in"
	"${PROJECT_BINARY_DIR}/fragment_shader.glsl"
    COPYONLY)

add_subdirectory(libs/glfw-3.3)

add_executable(tst
    main.cpp
    glad.c
    stb_image.cpp
    shader_loader.cpp
    oglrenderer.cpp
)

target_link_libraries(tst glfw)

add_custom_command(
    TARGET tst
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        "${PROJECT_BINARY_DIR}/libs/glfw-3.3/src/glfw3.dll"
        "${PROJECT_BINARY_DIR}/glfw3.dll")
