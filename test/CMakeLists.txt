if(EMBED_GLPK)
    list(APPEND EXTRA_TEST_CPP
        algorithms/test_CorrelationClustering.cpp
    )
endif()

rock_testsuite(test_graph suite.cpp
    test_EdgesVertices.cpp
    test_Observer.cpp
    test_Filters.cpp
    test_Graph.cpp
    test_SubGraph.cpp
    test_Percolation.cpp
    test_GraphIO.cpp
    test_GVGraph.cpp
    test_LemonGraph.cpp
    test_Iterators.cpp
    test_Utils.cpp
    algorithms/test_Algorithms.cpp
    algorithms/test_BFS.cpp
    algorithms/test_DFS.cpp
    algorithms/test_FloydWarshall.cpp
    algorithms/test_MinCostFlow.cpp
    algorithms/test_MultiCommodityMinCostFlow.cpp
    algorithms/test_LPSolver.cpp
    algorithms/test_LPProblem.cpp
    ${EXTRA_TEST_CPP}
    DEPS graph_analysis)
