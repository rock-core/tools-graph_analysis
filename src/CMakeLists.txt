rock_library(graph_analysis
    SOURCES Graph.cpp
    HEADERS EdgeProperty.hpp
        Filter.hpp
        CommonFilter.hpp
        Graph.hpp
        GraphAnalysis.hpp
        lemon/Graph.hpp
        snap/Graph.hpp
        VertexProperty.hpp
    DEPS_PKGCONFIG lemon snap
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt)

