find_package(Boost REQUIRED regex)
rock_library(graph_analysis
    SOURCES
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

