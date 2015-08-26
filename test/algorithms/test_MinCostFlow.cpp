#include <boost/test/unit_test.hpp>

#include <graph_analysis/algorithms/MinCostFlow.hpp>
#include <graph_analysis/lemon/DirectedGraph.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms_min_cost_flow)

BOOST_AUTO_TEST_CASE(network_simplex)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new WeightedVertex(1.0));
    Vertex::Ptr v1( new WeightedVertex(0));
    Vertex::Ptr v2( new WeightedVertex(-1.0));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);

    MinCostFlow::edge_t::Ptr e0(new MinCostFlow::edge_t());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);
    // lower bound
    e0->setWeight(0,0);
    // upper bound
    e0->setWeight(1,2);
    // cost
    e0->setWeight(1,10);

    MinCostFlow::edge_t::Ptr e1(new MinCostFlow::edge_t());
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v2);
    // lower bound
    e1->setWeight(0,0);
    // upper bound
    e1->setWeight(1,2);
    // cost
    e1->setWeight(1,10);

    graph->addEdge(e0);
    graph->addEdge(e1);

    //MinCostFlow::edge_t e2(new MinCostFlow::edge_t());
    //e2->setSourceVertex(v2);
    //e2->setTargetVertex(v0);

    MinCostFlow minCostFlow(graph, MinCostFlow::NetworkSimplex);
    minCostFlow.run();

}


BOOST_AUTO_TEST_SUITE_END();
