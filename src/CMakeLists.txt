rock_library(graph_analysis
    SOURCES Graph.cpp
    HEADERS EdgeProperty.hpp
        Graph.hpp
        GraphAnalysis.hpp
        LemonGraph.hpp
        SnapGraph.hpp
        VertexProperty.hpp
    DEPS_PKGCONFIG lemon snap
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis
    DEPS_PLAIN rt)

