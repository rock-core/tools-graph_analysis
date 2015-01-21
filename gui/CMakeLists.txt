# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
# Fix issue with graphviz in version 2.36
add_definitions(-DHAVE_STRING_H)
rock_init(omviz 0.1)
rock_standard_layout()
