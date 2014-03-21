rock_library(graph_analysis
    SOURCES Graph.cpp
        GraphElement.cpp
        GraphIO.cpp
        io/GexfWriter.cpp
    HEADERS Edge.hpp
        EdgeIterator.hpp
        Filter.hpp
        CommonFilters.hpp
        DirectedGraphInterface.hpp
        Graph.hpp
        GraphAnalysis.hpp
        GraphElement.hpp
        GraphIO.hpp
        io/GexfWriter.hpp
        lemon/ArcIterator.hpp
        lemon/Graph.hpp
        lemon/NodeIterator.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        Vertex.hpp
        VertexIterator.hpp
    DEPS_PKGCONFIG lemon snap base-lib gexf
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt)

