#include <boost/test/unit_test.hpp>

#include <graph_analysis/algorithms/MinCostFlow.hpp>
#include <graph_analysis/lemon/DirectedGraph.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms_min_cost_flow)

BOOST_AUTO_TEST_CASE(network_simplex)
{
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

    {
        graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

        WeightedVertex::Ptr u( new WeightedVertex(0.0));
        WeightedVertex::Ptr v( new WeightedVertex(0.0));
        WeightedVertex::Ptr s( new WeightedVertex(-30.0));
        WeightedVertex::Ptr t( new WeightedVertex(30.0));

        graph->addVertex(u);
        graph->addVertex(v);
        graph->addVertex(s);
        graph->addVertex(t);

        MinCostFlow::edge_t::Ptr e0(new MinCostFlow::edge_t());
        e0->setSourceVertex(s);
        e0->setTargetVertex(u);
        // lower bound
        e0->setWeight(0,0);
        // upper bound
        e0->setWeight(1,20);
        // cost
        e0->setWeight(2,0);

        MinCostFlow::edge_t::Ptr e1(new MinCostFlow::edge_t());
        e1->setSourceVertex(s);
        e1->setTargetVertex(v);
        // lower bound
        e1->setWeight(0,0);
        // upper bound
        e1->setWeight(1,10);
        // cost
        e1->setWeight(2,0);

        MinCostFlow::edge_t::Ptr e2(new MinCostFlow::edge_t());
        e2->setSourceVertex(u);
        e2->setTargetVertex(v);
        // lower bound
        e2->setWeight(0,0);
        // upper bound
        e2->setWeight(1,30);
        // cost
        e2->setWeight(2,0);

        MinCostFlow::edge_t::Ptr e3(new MinCostFlow::edge_t());
        e3->setSourceVertex(u);
        e3->setTargetVertex(t);
        // lower bound
        e3->setWeight(0,0);
        // upper bound
        e3->setWeight(1,10);
        // cost
        e3->setWeight(2,0);

        MinCostFlow::edge_t::Ptr e4(new MinCostFlow::edge_t());
        e4->setSourceVertex(v);
        e4->setTargetVertex(t);
        // lower bound
        e4->setWeight(0,0);
        // upper bound
        e4->setWeight(1,20);
        // cost
        e4->setWeight(2,0);


        graph->addEdge(e0);
        graph->addEdge(e1);
        graph->addEdge(e2);
        graph->addEdge(e3);
        graph->addEdge(e4);

        //MinCostFlow::edge_t e2(new MinCostFlow::edge_t());
        //e2->setSourceVertex(v2);
        //e2->setTargetVertex(v0);

        MinCostFlow minCostFlow(graph, MinCostFlow::NetworkSimplex);
        minCostFlow.run();
    }
}


BOOST_AUTO_TEST_SUITE_END();
