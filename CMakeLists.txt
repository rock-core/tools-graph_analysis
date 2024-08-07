# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.3)
project(graph_analysis VERSION 0.3 DESCRIPTION "A graph library (frontend) for modeling and analysis")
find_package(Rock)
# Currently requires RelWithDebInfo at minimum to build with lemon
set(CMAKE_BUILD_TYPE RelWithDebInfo)
# Required for graphviz
add_definitions(-DHAVE_STRING_H)
add_definitions(-Wall -g)
add_definitions(-DNUMERIC_DEPRECATE=1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
rock_init()
rock_feature(NOCURDIR)
rock_standard_layout()

install(DIRECTORY resources/icons
        DESTINATION share/graph_analysis/)
