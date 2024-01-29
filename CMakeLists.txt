cmake_minimum_required(VERSION 3.0)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(project_name VERSION 0.1.0 LANGUAGES C CXX)
# set(CXX_MAKE_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
# add_subdirectory(lib/stb)
add_subdirectory(lib/glm)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")

add_executable(project_name "${SOURCE_FILES}")

target_include_directories(project_name PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc/")
target_include_directories(project_name PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glfw glad glm)
