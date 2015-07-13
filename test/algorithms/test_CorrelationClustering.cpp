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
    Vertex::Ptr v3( new Vertex("3"));
    Vertex::Ptr v4( new Vertex("4"));
    Vertex::Ptr v5( new Vertex("5"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);
    graph->addVertex(v4);
    graph->addVertex(v5);

    WeightedEdge::Ptr e0(new WeightedEdge(2.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(-5.0));
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(-5.0));
    e2->setSourceVertex(v2);
    e2->setTargetVertex(v3);

    WeightedEdge::Ptr e3(new WeightedEdge(-1.0));
    e3->setSourceVertex(v3);
    e3->setTargetVertex(v0);

    WeightedEdge::Ptr e4(new WeightedEdge(2.0));
    e4->setSourceVertex(v3);
    e4->setTargetVertex(v4);    

    WeightedEdge::Ptr e5(new WeightedEdge(5.0));
    e5->setSourceVertex(v2);
    e5->setTargetVertex(v4);

    WeightedEdge::Ptr e6(new WeightedEdge(5.0));
    e6->setSourceVertex(v4);
    e6->setTargetVertex(v5);

    WeightedEdge::Ptr e7(new WeightedEdge(1.0));
    e7->setSourceVertex(v5);
    e7->setTargetVertex(v0);

    WeightedEdge::Ptr e8(new WeightedEdge(2.0));
    e8->setSourceVertex(v1);
    e8->setTargetVertex(v5);

    WeightedEdge::Ptr e9(new WeightedEdge(-5.0));
    e9->setSourceVertex(v5);
    e9->setTargetVertex(v2);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);
    graph->addEdge(e4);
    graph->addEdge(e5);
    graph->addEdge(e6);
    graph->addEdge(e7);
    graph->addEdge(e8);
    graph->addEdge(e9);

    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
        //BOOST_FAIL("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
        //BOOST_FAIL("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}

BOOST_AUTO_TEST_SUITE_END()
