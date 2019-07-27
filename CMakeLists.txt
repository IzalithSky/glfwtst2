cmake_minimum_required(VERSION 3.6)

project(glfwtst2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

configure_file(
	"${PROJECT_SOURCE_DIR}/resources/vertex_shader.glsl.in"
	"${PROJECT_BINARY_DIR}/vertex_shader.glsl"
    COPYONLY)

configure_file(
	"${PROJECT_SOURCE_DIR}/resources/fragment_shader.glsl.in"
	"${PROJECT_BINARY_DIR}/fragment_shader.glsl"
    COPYONLY)
    
configure_file(
	"${PROJECT_SOURCE_DIR}/resources/gato.png"
	"${PROJECT_BINARY_DIR}/gato.png"
    COPYONLY)

add_subdirectory(libs/glfw-3.3)

add_subdirectory(libs/glad)
include_directories(libs/glad)

add_subdirectory(libs/stb_image)
include_directories(libs/stb_image)

include_directories(libs/glm)

add_subdirectory(libs/assimp-4.0.1)

add_executable(tst
    src/main.cpp
    src/shader_loader.cpp
    src/oglrenderer.cpp
)

target_link_libraries(tst glfw)
target_link_libraries(tst glad)
target_link_libraries(tst stb_image)
target_link_libraries(tst assimp)

add_custom_command(
    TARGET tst
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        "${PROJECT_BINARY_DIR}/libs/glfw-3.3/src/glfw3.dll"
        "${PROJECT_BINARY_DIR}/glfw3.dll")
