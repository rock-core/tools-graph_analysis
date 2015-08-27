find_package(Boost REQUIRED regex)
rock_library(graph_analysis
    SOURCES
        algorithms/CorrelationClustering.cpp
        algorithms/FloydWarshall.cpp
        algorithms/MinCostFlow.cpp
        boost_graph/DirectedGraph.cpp
        Edge.cpp
        EdgeIterable.cpp
        EdgeIterator.cpp
        Filter.cpp
        filters/EdgeContextFilter.cpp
        filters/RegexFilters.cpp
        BaseGraph.cpp
        GraphElement.cpp
        GraphIO.cpp
        gui/EdgeItem.cpp
        gui/EdgeTypeManager.cpp
        gui/graphitem/edges/Simple.cpp
        gui/graphitem/Resource.cpp
        gui/graphitem/Simple.cpp
        gui/GraphWidget.cpp
        gui/Main.cpp
        gui/NodeItem.cpp
        gui/NodeTypeManager.cpp
        io/GexfWriter.cpp
        io/GraphvizWriter.cpp
        io/GVGraph.cpp
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
        VertexIterable.cpp
        VertexIterator.cpp
    HEADERS
        Algorithms.hpp
        algorithms/CorrelationClustering.hpp
        algorithms/DistanceMatrix.hpp
        algorithms/FloydWarshall.hpp
        algorithms/MinCostFlow.hpp
        boost_graph/DirectedGraph.hpp
        DirectedGraphInterface.hpp
        Edge.hpp
        EdgeIterable.hpp
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
        gui/graphitem/edges/EdgeLabel.hpp
        gui/graphitem/edges/Simple.hpp
        gui/graphitem/Label.hpp
        gui/graphitem/Resource.hpp
        gui/graphitem/Simple.hpp
        gui/GraphWidget.hpp
        gui/NodeItem.hpp
        gui/NodeTypeManager.hpp
        io/GexfWriter.hpp
        io/GraphvizWriter.hpp
        io/GVGraph.hpp
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
        VertexIterable.hpp
        VertexIterator.hpp
        WeightedEdge.hpp
        NWeightedEdge.hpp
        WeightedVertex.hpp
    DEPS_PKGCONFIG
        lemon snap base-lib gexf numeric gsl libgvc
    DEPS_PLAIN
        Boost_REGEX
    MOC
        gui/GraphWidget.hpp
    DEPS_PLAIN Boost_REGEX
    DEPS_PKGCONFIG QtGui
    UI
        gui/graph_analysis_gui.ui
)
target_link_libraries(graph_analysis glpk cgraph)
get_target_property(graph_analysis-linker_flags graph_analysis LINK_FLAGS)
if(graph_analysis-linker_flags)
    set(graph_analysis-linker_flags "${graph_analysis-linker_flags} -Wl,--exclude-libs=libsnap.a" )
else()
    set(graph_analysis-linker_flags "-Wl,--exclude-libs=libsnap.a" )
endif()
set_target_properties(graph_analysis PROPERTIES LINK_FLAGS ${graph_analysis-linker_flags})

rock_executable(graph_analysis-bm Benchmark.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt
    NOINSTALL)

rock_executable(graph_analysis-gui gui/Main.cpp
    DEPS graph_analysis
    NOINSTALL)
