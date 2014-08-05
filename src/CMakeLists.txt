rock_library(omviz
    SOURCES elasticnodes/Edge.cpp
      elasticnodes/GraphWidget.cpp
      elasticnodes/Node.cpp
    HEADERS elasticnodes/Edge.hpp
      elasticnodes/GraphWidget.hpp
      elasticnodes/Node.hpp
    MOC elasticnodes/Edge.hpp
      elasticnodes/GraphWidget.hpp
      elasticnodes/Node.hpp
)

rock_executable(omviz_elasticnodes elasticnodes/Main.cpp
    DEPS omviz)
rock_executable(omviz_bin Main.cpp
    DEPS omviz)

