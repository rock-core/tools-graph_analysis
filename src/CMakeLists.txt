rock_library(omviz
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(omviz_bin Main.cpp
    DEPS omviz)

