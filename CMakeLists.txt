# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.1)
find_package(Rock)
# Currently requires RelWithDebInfo at minimum to build with lemon
set(CMAKE_BUILD_TYPE RelWithDebInfo)
# Required for graphviz
add_definitions(-DHAVE_STRING_H)
add_definitions(-Wall -g)
add_definitions(-DNUMERIC_DEPRECATE=1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
rock_init(graph_analysis 0.2)
rock_standard_layout()

if(NOT TARGET graph_analysis_py)
    add_subdirectory(bindings/python)
endif()

install(DIRECTORY resources/icons
        DESTINATION share/graph_analysis/)
