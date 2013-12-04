rock_library(graph_analysis
    SOURCES Graph.cpp
    HEADERS EdgeProperty.hpp
        Graph.hpp
        VertexProperty.hpp
    DEPS_PKGCONFIG lemon
)

rock_executable(graph_analysis_bin Main.cpp
    DEPS graph_analysis)

