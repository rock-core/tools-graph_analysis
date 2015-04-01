#include <boost/test/unit_test.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/algorithms/FloydWarshall.hpp>
#include <graph_analysis/lemon/Graph.hpp>

using namespace graph_analysis;
using namespace graph_analysis::algorithms;

double getWeight(Edge::Ptr edge)
{
    WeightedEdge::Ptr weightedEdge = boost::dynamic_pointer_cast<WeightedEdge>(edge);
    return weightedEdge->getWeight();
}

BOOST_AUTO_TEST_SUITE(algorithms_floyd_warshall)
BOOST_AUTO_TEST_CASE(all_shortest_path)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);

    WeightedEdge::Ptr e0(new WeightedEdge(30));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(100.0));
    e1->setSourceVertex(v0);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(10.0));
    e2->setSourceVertex(v1);
    e2->setTargetVertex(v2);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);

    BOOST_REQUIRE_NO_THROW(FloydWarshall::allShortestPaths(graph, getWeight));

    DistanceMatrix distanceMatrix = FloydWarshall::allShortestPaths(graph, getWeight);
    double distance02 = distanceMatrix[std::pair<Vertex::Ptr, Vertex::Ptr>(v0,v2)];
    BOOST_REQUIRE_MESSAGE( distance02 == 40.0, "Shortest distance between v0 and v2 should be 40");

    double distance20 = distanceMatrix[std::pair<Vertex::Ptr, Vertex::Ptr>(v2,v0)];

    BOOST_REQUIRE_MESSAGE(distance20 == std::numeric_limits<double>::infinity(), "No connection from v2 to v1: cost infinite");

    // Create negative cycle
    WeightedEdge::Ptr e3(new WeightedEdge(-40.0));
    e3->setSourceVertex(v1);
    e3->setTargetVertex(v0);
    graph->addEdge(e3);

    BOOST_REQUIRE_THROW(FloydWarshall::allShortestPaths(graph, getWeight), std::runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()
