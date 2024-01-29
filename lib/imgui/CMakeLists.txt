cmake_minimum_required(VERSION 3.0)
project(imgui)

file(GLOB_RECURSE IMGUI_SOURCES *.cpp *.h)
add_library(imgui ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include/")