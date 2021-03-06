cmake_minimum_required(VERSION 2.8.4)

project(SoftwareRasterizer)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TESTS_DIR "${ROOT_DIR}/test")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ROOT_DIR}/cmake")
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE LIB_SRC_FILES "."
     "${ROOT_DIR}/src/*.cpp"
     "${ROOT_DIR}/src/*.c"
     "${ROOT_DIR}/src/*.h"
     "${ROOT_DIR}/include/*.h"
     )
include_directories(${SDL2_INCLUDE_DIR} "${ROOT_DIR}/include")

add_executable("test_wm" "${TESTS_DIR}/test_wm.c" ${LIB_SRC_FILES})
target_link_libraries("test_wm" ${SDL2_LIBRARY})

add_executable("test_primitives" "${TESTS_DIR}/test_primitives.c" ${LIB_SRC_FILES})
target_link_libraries("test_primitives" ${SDL2_LIBRARY})

add_executable("test_surfaces" "${TESTS_DIR}/test_surfaces.c" ${LIB_SRC_FILES})
target_link_libraries("test_surfaces" ${SDL2_LIBRARY})

add_executable("test_color_format" "${TESTS_DIR}/test_color_format.c" ${LIB_SRC_FILES})
target_link_libraries("test_color_format" ${SDL2_LIBRARY})

add_executable("test_blit" "${TESTS_DIR}/test_blit.c" ${LIB_SRC_FILES})
target_link_libraries("test_blit" ${SDL2_LIBRARY})
