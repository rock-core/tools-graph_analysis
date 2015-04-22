find_package(Boost REQUIRED regex)
rock_library(graph_analysis
    SOURCES
        algorithms/CorrelationClustering.cpp
        algorithms/FloydWarshall.cpp
        Edge.cpp
        EdgeIterator.cpp
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
        io/GraphvizWriter.cpp
        lemon/DirectedGraph.cpp
        lemon/DirectedSubGraph.cpp
        Percolation.cpp
        percolation/Event.cpp
        percolation/RandomNumberGenerator.cpp
        percolation/strategies/RandomDraw.cpp
        percolation/strategies/UniformRandomEdgeRemoval.cpp
        percolation/strategies/UniformRandomVertexRemoval.cpp
        snap/DirectedSubGraph.cpp
        snap/DirectedGraph.cpp
        SubGraph.cpp
        Vertex.cpp
        VertexIterator.cpp
        WeightedEdge.cpp
    HEADERS
        Algorithms.hpp
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
        io/GraphvizWriter.hpp
        lemon/ArcIterator.hpp
        lemon/DirectedGraph.hpp
        lemon/DirectedSubGraph.hpp
        lemon/Graph.hpp
        lemon/NodeIterator.hpp
        Percolation.hpp
        percolation/Event.hpp
        percolation/RandomNumberGenerator.hpp
        percolation/strategies/RandomDraw.hpp
        percolation/strategies/UniformRandomEdgeRemoval.hpp
        percolation/strategies/UniformRandomVertexRemoval.hpp
        snap/DirectedSubGraph.hpp
        snap/DirectedGraph.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        SubGraph.hpp
        SubGraphImpl.hpp
        TypedGraph.hpp
        Vertex.hpp
        VertexIterator.hpp
        WeightedEdge.hpp
    DEPS_PKGCONFIG
        lemon snap base-lib gexf numeric gsl
    DEPS_PLAIN
        Boost_REGEX
    MOC
        gui/GraphWidget.hpp
    DEPS_PLAIN Boost_REGEX
    DEPS_PKGCONFIG QtGui
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
