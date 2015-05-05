#include <boost/test/unit_test.hpp>
#include <graph_analysis/io/GVGraph.hpp>

using namespace graph_analysis;

struct GVFixture
{
    BaseGraph::Ptr baseGraph;
    io::GVGraph graph;

    GVFixture()
        : baseGraph( BaseGraph::getInstance(BaseGraph::LEMON_DIRECTED_GRAPH) )
        , graph(baseGraph, "GVGraphWidget")
    {
        Vertex::Ptr v1(new Vertex("vertex-1"));
        Vertex::Ptr v2(new Vertex("vertex-2"));
        Vertex::Ptr v3(new Vertex("vertex-3"));
        Vertex::Ptr v4(new Vertex("vertex-4"));

        Edge::Ptr e0(new Edge("edge-0"));
        e0->setSourceVertex(v1);
        e0->setTargetVertex(v2);

        Edge::Ptr e1(new Edge("edge-1"));
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v4);

        Edge::Ptr e2(new Edge("edge-2"));
        e2->setSourceVertex(v2);
        e2->setTargetVertex(v3);

        Edge::Ptr e3(new Edge("edge-3"));
        e3->setSourceVertex(v4);
        e3->setTargetVertex(v2);

        Edge::Ptr e4(new Edge("edge-4"));
        e4->setSourceVertex(v2);
        e4->setTargetVertex(v4);

        baseGraph->addVertex(v1);
        baseGraph->addVertex(v2);
        baseGraph->addVertex(v3);
        baseGraph->addVertex(v4);

        baseGraph->addEdge(e0);
        baseGraph->addEdge(e1);
        baseGraph->addEdge(e2);
        baseGraph->addEdge(e3);
        baseGraph->addEdge(e4);

        graph.addNode(v1);
        graph.addNode(v2);
        graph.addNode(v3);
        graph.addNode(v4);

        graph.addEdge(e1);
        graph.addEdge(e2);
        graph.addEdge(e3);
        graph.addEdge(e4);
    }
};

BOOST_AUTO_TEST_SUITE(graphviz)

/*
 * Note:
 * ----
 * 
 * These tests imply that clearNodes() shall not be called before clearEdges() -> otherwise exceptions are raised internally
 */

BOOST_FIXTURE_TEST_CASE(clear_edges_and_nodes, GVFixture)
{

    BOOST_TEST_MESSAGE("it_should_clear_edges_and_nodes: graph.clearEdges()");
    BOOST_REQUIRE_NO_THROW( graph.clearEdges() );
    BOOST_TEST_MESSAGE("it_should_clear_edges_and_nodes: graph.clearNodes()");
    BOOST_REQUIRE_NO_THROW( graph.clearNodes() );
}

BOOST_FIXTURE_TEST_CASE(clear_nodes_and_edges, GVFixture)
{
    BOOST_TEST_MESSAGE("it_should_clear_nodes_and_edges: graph.clearNodes()");
    graph.clearNodes();
    BOOST_TEST_MESSAGE("it_should_clear_nodes_and_edges: graph.clearEdges()");
    graph.clearEdges();
}

BOOST_FIXTURE_TEST_CASE(clear_nodes, GVFixture)
{
    BOOST_TEST_MESSAGE("it_should_clear_nodes: graph.clearNodes()");
    BOOST_REQUIRE_NO_THROW(graph.clearNodes());
}

BOOST_FIXTURE_TEST_CASE(clear_edges, GVFixture)
{
    BOOST_TEST_MESSAGE("it_should_clear_edges: graph.clearEdges()");
    BOOST_REQUIRE_NO_THROW( graph.clearEdges() );
}

BOOST_AUTO_TEST_SUITE_END()
