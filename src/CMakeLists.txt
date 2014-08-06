rock_library(omviz
    SOURCES graphwidget/EdgeItem.cpp
      graphwidget/GraphWidget.cpp
      graphwidget/NodeItem.cpp
      graphwidget/NodeTypeManager.cpp
      graphwidget/graphitem/Simple.cpp
      graphwidget/graphitem/Resource.cpp
      IRINode.cpp
    HEADERS graphwidget/EdgeItem.hpp
      graphwidget/GraphWidget.hpp
      graphwidget/NodeItem.hpp
      graphwidget/NodeTypeManager.hpp
      graphwidget/graphitem/Simple.hpp
      graphwidget/graphitem/Resource.hpp
      IRINode.hpp
    MOC graphwidget/GraphWidget.hpp
    DEPS_PKGCONFIG graph_analysis owl_om
)

rock_executable(omviz_bin graphwidget/Main.cpp
    DEPS omviz)

rock_library(omviz_elasticnodes
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
rock_executable(omviz_elasticnodes_bin elasticnodes/Main.cpp
    DEPS omviz_elasticnodes)

