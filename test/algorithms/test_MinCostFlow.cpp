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

        MinCostFlow::vertex_t::Ptr v0( new MinCostFlow::vertex_t(1.0));
        MinCostFlow::vertex_t::Ptr v1( new MinCostFlow::vertex_t(0));
        MinCostFlow::vertex_t::Ptr v2( new MinCostFlow::vertex_t(-1.0));

        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);

        MinCostFlow::edge_t::Ptr e0(new MinCostFlow::edge_t());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);
        // lower bound
        e0->setWeight(0, 0);
        // upper bound
        e0->setWeight(2, 1);
        // cost
        e0->setWeight(1.0, 2);

        MinCostFlow::edge_t::Ptr e1(new MinCostFlow::edge_t());
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v2);
        // lower bound
        e1->setWeight(0, 0);
        // upper bound
        e1->setWeight(2, 1);
        // cost
        e1->setWeight(3.0, 2);

        graph->addEdge(e0);
        graph->addEdge(e1);

        //MinCostFlow::edge_t e2(new MinCostFlow::edge_t());
        //e2->setSourceVertex(v2);
        //e2->setTargetVertex(v0);

        MinCostFlow minCostFlow(graph, MinCostFlow::NetworkSimplex);
        uint32_t cost =  minCostFlow.run();
        BOOST_REQUIRE_MESSAGE(cost == 4, "Min flow should be 4, was " << cost);
    }

    {
        graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

        MinCostFlow::vertex_t::Ptr u( new MinCostFlow::vertex_t(0.0));
        MinCostFlow::vertex_t::Ptr v( new MinCostFlow::vertex_t(0.0));
        MinCostFlow::vertex_t::Ptr s( new MinCostFlow::vertex_t(10.0));
        MinCostFlow::vertex_t::Ptr t( new MinCostFlow::vertex_t(-10.0));

        graph->addVertex(u);
        graph->addVertex(v);
        graph->addVertex(s);
        graph->addVertex(t);

        MinCostFlow::edge_t::Ptr e0(new MinCostFlow::edge_t());
        e0->setSourceVertex(s);
        e0->setTargetVertex(u);
        // lower bound
        e0->setWeight(0, 0);
        // upper bound
        e0->setWeight(20, 1);
        // cost
        e0->setWeight(1.0, 2);

        MinCostFlow::edge_t::Ptr e1(new MinCostFlow::edge_t());
        e1->setSourceVertex(s);
        e1->setTargetVertex(v);
        // lower bound
        e1->setWeight(0, 0);
        // upper bound
        e1->setWeight(10, 1);
        // cost
        e1->setWeight(1.0, 2);

        MinCostFlow::edge_t::Ptr e2(new MinCostFlow::edge_t());
        e2->setSourceVertex(u);
        e2->setTargetVertex(v);
        // lower bound
        e2->setWeight(0,0);
        // upper bound
        e2->setWeight(30, 1);
        // cost
        e2->setWeight(1.0, 2);

        MinCostFlow::edge_t::Ptr e3(new MinCostFlow::edge_t());
        e3->setSourceVertex(u);
        e3->setTargetVertex(t);
        // lower bound
        e3->setWeight(0, 0);
        // upper bound
        e3->setWeight(10, 1);
        // cost
        e3->setWeight(1.0, 2);

        MinCostFlow::edge_t::Ptr e4(new MinCostFlow::edge_t());
        e4->setSourceVertex(v);
        e4->setTargetVertex(t);
        // lower bound
        e4->setWeight(0, 0);
        // upper bound
        e4->setWeight(20, 1);
        // cost
        e4->setWeight(1.0, 2);


        graph->addEdge(e0);
        graph->addEdge(e1);
        graph->addEdge(e2);
        graph->addEdge(e3);
        graph->addEdge(e4);

        //MinCostFlow::edge_t e2(new MinCostFlow::edge_t());
        //e2->setSourceVertex(v2);
        //e2->setTargetVertex(v0);

        MinCostFlow minCostFlow(graph, MinCostFlow::NetworkSimplex);
        uint32_t cost =  minCostFlow.run();
        BOOST_REQUIRE_MESSAGE(cost == 20, "Min flow should be 20, was " << cost);
    }
}


BOOST_AUTO_TEST_SUITE_END();
