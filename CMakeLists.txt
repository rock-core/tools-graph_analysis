# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
# Required for graphviz
add_definitions(-DHAVE_STRING_H)
rock_init(graph_analysis 0.1)
rock_standard_layout()
