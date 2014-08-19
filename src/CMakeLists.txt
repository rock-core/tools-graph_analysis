find_package(Boost REQUIRED regex)

rock_library(omviz
    SOURCES graphwidget/EdgeItem.cpp
      graphwidget/EdgeTypeManager.cpp
      graphwidget/GraphWidget.cpp
      graphwidget/NodeItem.cpp
      graphwidget/NodeTypeManager.cpp
      graphwidget/graphitem/Simple.cpp
      graphwidget/graphitem/Resource.cpp
      graphwidget/graphitem/edges/Simple.cpp
      graphwidget/GVGraph.cpp
      planningwidget/PlanningWidget.cpp
      IRIEdge.cpp
      IRINode.cpp
      MainWindow.cpp
    HEADERS graphwidget/EdgeItem.hpp
      graphwidget/EdgeTypeManager.hpp
      graphwidget/GraphWidget.hpp
      graphwidget/NodeItem.hpp
      graphwidget/NodeTypeManager.hpp
      graphwidget/filters/Filters.hpp
      graphwidget/graphitem/Label.hpp
      graphwidget/graphitem/Simple.hpp
      graphwidget/graphitem/Resource.hpp
      graphwidget/graphitem/edges/Simple.hpp
      graphwidget/GVGraph.hpp
      planningwidget/PlanningWidget.hpp
      IRIEdge.cpp
      IRINode.hpp
      MainWindow.hpp
    MOC
        MainWindow.hpp
        graphwidget/GraphWidget.hpp
        planningwidget/PlanningWidget.hpp
    DEPS_PKGCONFIG graph_analysis owl_om
    DEPS_PLAIN Boost_REGEX
    UI
        ui/omviz.ui
        ui/planning_widget.ui
)
target_link_libraries(omviz gvc)

rock_executable(omviz_main Main.cpp
    DEPS omviz)

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

QT4_ADD_RESOURCES(Resource_SOURCES diagramscene/diagramscene.qrc)

rock_library(omviz_diagramscene
    SOURCES 
        diagramscene/arrow.cpp
        diagramscene/diagramitem.cpp
        diagramscene/diagramscene.cpp
        diagramscene/diagramtextitem.cpp
        diagramscene/mainwindow.cpp
    HEADERS 
        diagramscene/arrow.h
        diagramscene/diagramitem.h
        diagramscene/diagramscene.h
        diagramscene/diagramtextitem.h
        diagramscene/mainwindow.h
    MOC 
        diagramscene/diagramitem.h
        diagramscene/diagramscene.h
        diagramscene/diagramtextitem.h
        diagramscene/mainwindow.h
)

rock_executable(omviz_diagramscene_bin diagramscene/main.cpp ${Resource_SOURCES}
    DEPS omviz_diagramscene)

