find_package(Boost REQUIRED regex)
rock_library(graph_analysis
    SOURCES
        Filter.cpp
        filters/RegexFilters.cpp
        Graph.cpp
        GraphElement.cpp
        GraphIO.cpp
        io/GexfWriter.cpp
        lemon/Graph.cpp
    HEADERS
        DirectedGraphInterface.hpp
        Edge.hpp
        EdgeIterator.hpp
        Filter.hpp
        filters/CommonFilters.hpp
        filters/RegexFilters.hpp
        Graph.hpp
        GraphAnalysis.hpp
        GraphElement.hpp
        GraphIO.hpp
        GraphView.hpp
        io/GexfWriter.hpp
        lemon/ArcIterator.hpp
        lemon/Graph.hpp
        lemon/NodeIterator.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        Vertex.hpp
        VertexIterator.hpp
    DEPS_PKGCONFIG
        lemon snap base-lib gexf
    DEPS_PLAIN
        Boost_REGEX
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt)

rock_library(graph_analysis_gui
    SOURCES
      gui/EdgeItem.cpp
      gui/EdgeTypeManager.cpp
      gui/GraphWidget.cpp
      gui/NodeItem.cpp
      gui/NodeTypeManager.cpp
      gui/graphitem/Simple.cpp
      gui/graphitem/Resource.cpp
      gui/graphitem/edges/Simple.cpp
      gui/GVGraph.cpp
      #Utils.cpp
    HEADERS
      gui/EdgeItem.hpp
      gui/EdgeTypeManager.hpp
      gui/GraphWidget.hpp
      gui/NodeItem.hpp
      gui/NodeTypeManager.hpp
      gui/graphitem/Label.hpp
      gui/graphitem/Simple.hpp
      gui/graphitem/Resource.hpp
      gui/graphitem/edges/Simple.hpp
      gui/GVGraph.hpp
      #Utils.hpp
    MOC
        gui/GraphWidget.hpp
    DEPS graph_analysis
    DEPS_PLAIN Boost_REGEX
    UI
        gui/graph_analysis_gui.ui
)
target_link_libraries(graph_analysis_gui gvc)
rock_executable(graph_analysis_gui-bin gui/Main.cpp
    DEPS graph_analysis_gui)


