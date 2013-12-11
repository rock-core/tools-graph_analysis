rock_library(graph_analysis
    SOURCES Graph.cpp
        GraphElement.cpp
    HEADERS Edge.hpp
        EdgeIterator.hpp
        Filter.hpp
        CommonFilter.hpp
        Graph.hpp
        GraphAnalysis.hpp
        lemon/Graph.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        Vertex.hpp
        VertexIterator.hpp
    DEPS_PKGCONFIG lemon snap base-lib
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt)

