find_package(Boost REQUIRED regex)
rock_library(graph_analysis
    SOURCES
        algorithms/CorrelationClustering.cpp
        algorithms/FloydWarshall.cpp
        Edge.cpp
        Filter.cpp
        filters/EdgeContextFilter.cpp
        filters/RegexFilters.cpp
        BaseGraph.cpp
        GraphElement.cpp
        GraphIO.cpp
        gui/EdgeItem.cpp
        gui/EdgeTypeManager.cpp
        gui/GraphWidget.cpp
        gui/NodeItem.cpp
        gui/NodeTypeManager.cpp
        gui/graphitem/Simple.cpp
        gui/graphitem/Resource.cpp
        gui/graphitem/edges/Simple.cpp
        gui/GVGraph.cpp
        io/GexfWriter.cpp
        lemon/Graph.cpp
        Vertex.cpp
        WeightedEdge.cpp
    HEADERS
        algorithms/CorrelationClustering.hpp
        algorithms/DistanceMatrix.hpp
        algorithms/FloydWarshall.hpp
        DirectedGraphInterface.hpp
        Edge.hpp
        EdgeIterator.hpp
        Filter.hpp
        filters/CommonFilters.hpp
        filters/EdgeContextFilter.hpp
        filters/RegexFilters.hpp
        BaseGraph.hpp
        Graph.hpp
        GraphAnalysis.hpp
        GraphElement.hpp
        GraphIO.hpp
        GraphView.hpp
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
        io/GexfWriter.hpp
        lemon/ArcIterator.hpp
        lemon/Graph.hpp
        lemon/NodeIterator.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        TypedGraph.hpp
        TypedSubGraph.hpp
        Vertex.hpp
        VertexIterator.hpp
        WeightedEdge.hpp
    DEPS_PKGCONFIG
        lemon snap base-lib gexf numeric
    DEPS_PLAIN
        Boost_REGEX
    MOC
        gui/GraphWidget.hpp
    UI
        gui/graph_analysis_gui.ui
)
target_link_libraries(graph_analysis glpk gvc)

rock_executable(graph_analysis-bm Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt
    NOINSTALL)

rock_executable(graph_analysis-gui gui/Main.cpp
    DEPS graph_analysis
    NOINSTALL)

