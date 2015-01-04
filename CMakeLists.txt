cmake_minimum_required(VERSION 2.8.4)

set(PROJECT_NAME SoftwareRasterizer)
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(SoftwareRasterizer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SRC_DIR}/cmake")
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "."
     "${PROJECT_SRC_DIR}/src/*.cpp"
     "${PROJECT_SRC_DIR}/src/*.c"
     "${PROJECT_SRC_DIR}/src/*.h"
     "${PROJECT_SRC_DIR}/include/*.h"
     )
include_directories(${SDL2_INCLUDE_DIR} "${PROJECT_SRC_DIR}/include")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

