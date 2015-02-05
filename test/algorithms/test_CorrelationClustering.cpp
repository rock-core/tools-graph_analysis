#include <boost/test/unit_test.hpp>

#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/algorithms/CorrelationClustering.hpp>
#include <graph_analysis/WeightedEdge.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms_correlation_clustering)

double getWeight(Edge::Ptr edge)
{
    WeightedEdge::Ptr weightedEdge = boost::dynamic_pointer_cast<WeightedEdge>(edge);
    return weightedEdge->getWeight();
}

BOOST_AUTO_TEST_CASE(it_should_compute_cluster_correlation)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);

    WeightedEdge::Ptr e0(new WeightedEdge(1.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(-1.0));
    e1->setSourceVertex(v0);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(-1.0));
    e2->setSourceVertex(v1);
    e2->setTargetVertex(v2);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);

    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}

BOOST_AUTO_TEST_SUITE_END()
