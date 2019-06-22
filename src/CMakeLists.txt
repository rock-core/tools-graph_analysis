find_package(Boost REQUIRED filesystem system serialization)
find_package(SCIP)

if(EMBED_GLPK)
    add_definitions(-DEMBED_GLPK)
    list(APPEND EXTRA_HPP
            algorithms/lp/embedded/GLPKSolver.hpp
            algorithms/CorrelationClustering.hpp
    )
    list(APPEND EXTRA_CPP
        algorithms/lp/embedded/GLPKSolver.cpp
        algorithms/CorrelationClustering.cpp
    )
endif()

if(EMBED_SCIP)
if(SCIP_FOUND)
    list(APPEND EXTRA_HPP
            algorithms/lp/embedded/ScipSolver.hpp
            algorithms/lp/embedded/SoplexSolver.hpp
    )
    list(APPEND EXTRA_CPP
        algorithms/lp/embedded/ScipSolver.cpp
        algorithms/lp/embedded/SoplexSolver.cpp
    )

    add_definitions(-DWITH_SCIP)
else(SCIP_FOUND)
    message(STATUS "Embedding SCIP requested, but library is not installed")
endif(SCIP_FOUND)
endif(EMBED_SCIP)

rock_library(graph_analysis
    SOURCES
        Attribute.cpp
        AttributeManager.cpp
        BaseGraph.cpp
        BipartiteGraph.cpp
        DirectedHyperEdge.cpp
        Edge.cpp
        EdgeIterable.cpp
        EdgeIterator.cpp
        EdgeTypeManager.cpp
        Filter.cpp
        GraphElement.cpp
        GraphIO.cpp
        HyperEdge.cpp
        Percolation.cpp
        SubGraph.cpp
        Vertex.cpp
        VertexIterable.cpp
        VertexIterator.cpp
        VertexTypeManager.cpp
        TransactionObserver.cpp
        algorithms/BFS.cpp
        algorithms/ConstraintViolation.cpp
        algorithms/DFS.cpp
        algorithms/FloydWarshall.cpp
        algorithms/lp/Problem.cpp
        algorithms/lp/Solution.cpp
        algorithms/LPSolver.cpp
        algorithms/lp/CBCSolver.cpp
        algorithms/lp/CLPSolver.cpp
        algorithms/lp/CommandlineSolver.cpp
        algorithms/lp/GLPKSolver.cpp
        algorithms/lp/ScipSolver.cpp
        algorithms/MinCostFlow.cpp
        algorithms/MultiCommodityEdge.cpp
        algorithms/MultiCommodityMinCostFlow.cpp
        algorithms/MultiCommodityVertex.cpp
        algorithms/Visitor.cpp
        boost_graph/DirectedGraph.cpp
        boost_graph/DirectedSubGraph.cpp
        filters/EdgeContextFilter.cpp
        filters/RegexFilters.cpp
        gui/ActionCommander.cpp
        gui/BaseGraphView/AddEdgeDialog.cpp
        gui/BaseGraphView/AddVertexDialog.cpp
        gui/BaseGraphView/BaseGraphView.cpp
        gui/EdgeItemBase.cpp
        gui/EdgeItemSimple.cpp
        gui/EdgeItemTypeManager.cpp
        gui/EdgeMimeData.cpp
        gui/FilterItem.cpp
        gui/FilterManager.cpp
        gui/GraphAnalysisGui.cpp
        gui/GraphLayout.cpp
        gui/GraphLayoutManager.cpp
        gui/GraphWidget.cpp
        gui/GraphWidgetManager.cpp
        gui/IconManager.cpp
        gui/VertexItemTypeManager.cpp
        gui/Player.cpp
        gui/PlayerConfiguration.cpp
        gui/QBaseGraph.cpp
        gui/VertexItemBase.cpp
        gui/VertexItemSimple.cpp
        gui/WidgetManager.cpp
        gui/dialogs/AddFilterDialog.cpp
        gui/dialogs/AddGraphElement.cpp
        gui/dialogs/CustomDialog.cpp
        gui/dialogs/GridLayout.cpp
        gui/dialogs/IODialog.cpp
        gui/dialogs/RenameFilterDialog.cpp
        gui/dialogs/SwapFiltersDialog.cpp
        gui/items/Feature.cpp
        gui/items/Label.cpp
        gui/layouts/GVLayout.cpp
        gui/layouts/GridLayout.cpp
        io/GVGraph.cpp
        io/GexfReader.cpp
        io/GexfWriter.cpp
        io/GraphMLReader.cpp
        io/GraphMLWriter.cpp
        io/GraphvizGridStyle.cpp
        io/GraphvizWriter.cpp
        io/YamlReader.cpp
        io/YamlWriter.cpp
        lemon/DirectedGraph.cpp
        lemon/DirectedSubGraph.cpp
        percolation/Event.cpp
        percolation/RandomNumberGenerator.cpp
        percolation/strategies/RandomDraw.cpp
        percolation/strategies/UniformRandomEdgeRemoval.cpp
        percolation/strategies/UniformRandomVertexRemoval.cpp
        snap/DirectedGraph.cpp
        snap/DirectedSubGraph.cpp
        utils/MD5.cpp
        utils/Filesystem.cpp
        ${EXTRA_CPP}
    HEADERS
        Attribute.hpp
        AttributeManager.hpp
        Algorithms.hpp
        BaseGraph.hpp
        BaseGraphObserver.hpp
        TransactionObserver.hpp
        BaseIterable.hpp
        BipartiteGraph.hpp
        DirectedGraphInterface.hpp
        DirectedHyperEdge.hpp
        Edge.hpp
        EdgeIterable.hpp
        EdgeIterator.hpp
        EdgeRegistration.hpp
        EdgeTypeManager.hpp
        Filter.hpp
        Graph.hpp
        GraphAnalysis.hpp
        GraphElement.hpp
        GraphIO.hpp
        HyperEdge.hpp
        NWeighted.hpp
        NWeightedEdge.hpp
        Percolation.hpp
        SharedPtr.hpp
        SubGraph.hpp
        SubGraphImpl.hpp
        TypedGraph.hpp
        Vertex.hpp
        VertexIterable.hpp
        VertexIterator.hpp
        VertexRegistration.hpp
        VertexTypeManager.hpp
        WeightedEdge.hpp
        WeightedVertex.hpp
        algorithms/BFS.hpp
        algorithms/BFSVisitor.hpp
        algorithms/ConstraintViolation.hpp
        algorithms/DistanceMatrix.hpp
        algorithms/DFS.hpp
        algorithms/DFSVisitor.hpp
        algorithms/FloydWarshall.hpp
        algorithms/lp/Problem.hpp
        algorithms/lp/Solution.hpp
        algorithms/LPSolver.hpp
        algorithms/lp/CBCSolver.hpp
        algorithms/lp/CLPSolver.hpp
        algorithms/lp/CommandlineSolver.hpp
        algorithms/lp/GLPKSolver.hpp
        algorithms/lp/ScipSolver.hpp
        algorithms/MinCostFlow.hpp
        algorithms/MultiCommodityEdge.hpp
        algorithms/MultiCommodityMinCostFlow.hpp
        algorithms/MultiCommodityVertex.hpp
        algorithms/Skipper.hpp
        algorithms/Visitor.hpp
        boost_graph/DirectedGraph.hpp
        boost_graph/DirectedSubGraph.hpp
        filters/CommonFilters.hpp
        filters/EdgeContextFilter.hpp
        filters/RegexFilters.hpp
        gui/ActionCommander.hpp
        gui/BaseGraphView/AddEdgeDialog.hpp
        gui/BaseGraphView/AddVertexDialog.hpp
        gui/BaseGraphView/BaseGraphView.hpp
        gui/EdgeItemBase.hpp
        gui/EdgeItemSimple.hpp
        gui/EdgeItemTypeManager.hpp
        gui/EdgeMimeData.hpp
        gui/FilterItem.hpp
        gui/FilterManager.hpp
        gui/GraphAnalysisGui.hpp
        gui/GraphLayout.hpp
        gui/GraphLayoutManager.hpp
        gui/GraphWidget.hpp
        gui/GraphWidgetManager.hpp
        gui/GraphicsItemTypes.hpp
        gui/IconManager.hpp
        gui/VertexItemTypeManager.hpp
        gui/Player.hpp
        gui/PlayerConfiguration.hpp
        gui/QBaseGraph.hpp
        gui/RegisterQtMetatypes.hpp
        gui/VertexItemBase.hpp
        gui/VertexItemSimple.hpp
        gui/WidgetManager.hpp
        gui/dialogs/AddFilterDialog.hpp
        gui/dialogs/AddGraphElement.hpp
        gui/dialogs/CustomDialog.hpp
        gui/dialogs/GridLayout.hpp
        gui/dialogs/IODialog.hpp
        gui/dialogs/RenameFilterDialog.hpp
        gui/dialogs/SwapFiltersDialog.hpp
        gui/items/Feature.hpp
        gui/items/Label.hpp
        gui/layouts/GVLayout.hpp
        gui/layouts/GridLayout.hpp
        io/GVGraph.hpp
        io/GexfReader.hpp
        io/GexfWriter.hpp
        io/GraphMLReader.hpp
        io/GraphMLWriter.hpp
        io/GraphvizStyle.hpp
        io/GraphvizGridStyle.hpp
        io/GraphvizWriter.hpp
        io/Serialization.hpp
        io/YamlReader.hpp
        io/YamlWriter.hpp
        lemon/ArcIterator.hpp
        lemon/DirectedGraph.hpp
        lemon/DirectedSubGraph.hpp
        lemon/Graph.hpp
        lemon/NodeIterator.hpp
        percolation/Event.hpp
        percolation/RandomNumberGenerator.hpp
        percolation/strategies/RandomDraw.hpp
        percolation/strategies/UniformRandomEdgeRemoval.hpp
        percolation/strategies/UniformRandomVertexRemoval.hpp
        snap/DirectedGraph.hpp
        snap/DirectedSubGraph.hpp
        snap/EdgeIterator.hpp
        snap/Graph.hpp
        snap/NodeIterator.hpp
        utils/MD5.hpp
        utils/Filesystem.hpp
        ${EXTRA_HPP}
    DEPS_PKGCONFIG
        lemon snap base-lib gexf numeric libgvc utilmm yaml-cpp
    DEPS_PLAIN
        Boost_SERIALIZATION
        Boost_FILESYSTEM
    MOC
        gui/GraphAnalysisGui.hpp
        gui/QBaseGraph.hpp
        gui/FilterManager.hpp
        gui/GraphWidget.hpp
        gui/GraphWidgetManager.hpp
        gui/dialogs/AddFilterDialog.hpp
        gui/dialogs/AddGraphElement.hpp
        gui/dialogs/GridLayout.hpp
        gui/dialogs/RenameFilterDialog.hpp
        gui/dialogs/SwapFiltersDialog.hpp
        gui/BaseGraphView/BaseGraphView.hpp
        gui/BaseGraphView/AddVertexDialog.hpp
        gui/BaseGraphView/AddEdgeDialog.hpp
        gui/Player.hpp
    DEPS_PKGCONFIG QtGui QtSvg
    UI
        gui/GraphAnalysisGui.ui
        gui/BaseGraphView/AddVertexDialog.ui
        gui/BaseGraphView/AddEdgeDialog.ui
        gui/dialogs/add_filter_dialog_gui.ui
        gui/dialogs/add_graph_element.ui
        gui/dialogs/graph_analysis_gui.ui
        gui/dialogs/grid_layout.ui
        gui/dialogs/property_dialog_gui.ui
        gui/dialogs/swap_filters_dialog_gui.ui
)
target_link_libraries(graph_analysis cgraph gomp)

if(EMBED_GLPK)
    target_link_libraries(graph_analysis glpk)
endif(EMBED_GLPK)

if(EMBED_SCIP)
if(SCIP_FOUND)
    message(STATUS "Linking SCIP Library: ${SCIP_LIBRARIES}")
    target_link_libraries(graph_analysis ${SCIP_LIBRARIES})
endif()
endif()

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
        Boost_FILESYSTEM
        Boost_SYSTEM
    NOINSTALL)

rock_executable(graph_analysis-gui gui/main.cpp
    DEPS graph_analysis
    )

rock_executable(graph_analysis-reader utils/GraphReaderMain.cpp
    DEPS graph_analysis
    )
