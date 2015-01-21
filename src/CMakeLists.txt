find_package(Boost REQUIRED regex)

rock_library(omviz
    SOURCES
      graphwidget/EdgeItem.cpp
      graphwidget/EdgeTypeManager.cpp
      graphwidget/GraphWidget.cpp
      graphwidget/NodeItem.cpp
      graphwidget/NodeTypeManager.cpp
      graphwidget/graphitem/Simple.cpp
      graphwidget/graphitem/Resource.cpp
      graphwidget/graphitem/edges/Simple.cpp
      graphwidget/GVGraph.cpp
      MainWindow.cpp
      Utils.cpp
    HEADERS
      graphwidget/EdgeItem.hpp
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
      MainWindow.hpp
      Utils.hpp
    MOC
        MainWindow.hpp
        graphwidget/GraphWidget.hpp
    DEPS_PKGCONFIG graph_analysis
    DEPS_PLAIN Boost_REGEX
    UI
        ui/omviz.ui
)
target_link_libraries(omviz gvc)

rock_executable(omviz_bin graphwidget/Main.cpp
    DEPS omviz)

rock_executable(omviz_main Main.cpp
    DEPS omviz)
