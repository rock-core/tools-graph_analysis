#include <boost/test/unit_test.hpp>
#include <graph_analysis/BaseGraph.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms)

BOOST_AUTO_TEST_CASE(connected)
{
    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());
    Edge::Ptr e0( new Edge(v0,v1));
    Edge::Ptr e1( new Edge(v1,v0));

    BaseGraph::Ptr graph = BaseGraph::getInstance(BaseGraph::LEMON_DIRECTED_GRAPH);

    graph->addVertex(v0);
    graph->addVertex(v1);
    BOOST_REQUIRE_MESSAGE(!graph->isConnected(), "Graph with two vertices is not connected");

    graph->addEdge(e0);
    graph->addEdge(e1);
    BOOST_REQUIRE_MESSAGE(graph->isConnected(), "Graph with two vertices and an edge is connected");

}

BOOST_AUTO_TEST_SUITE_END()



