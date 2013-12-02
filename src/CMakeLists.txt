rock_library(graph
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(graph_bin Main.cpp
    DEPS graph)

