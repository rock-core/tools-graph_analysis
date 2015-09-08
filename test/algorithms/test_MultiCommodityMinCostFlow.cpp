#include <boost/test/unit_test.hpp>

#include <graph_analysis/algorithms/MultiCommodityMinCostFlow.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms)

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_0)
{
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance();
        uint32_t commodities = 3;
        uint32_t edgeCapacityUpperBound = 30;

        MultiCommodityMinCostFlow::vertex_t::Ptr v0( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v1( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v2( new MultiCommodityMinCostFlow::vertex_t(commodities));

        for(size_t i = 0; i < commodities; ++i)
        {
            // Source node
            v0->setCommoditySupply(i, 10);
            BOOST_REQUIRE_MESSAGE(v0->getCommoditySupply(i) == 10, "Supply set to 10");
            // Target node
            v2->setCommoditySupply(i, -10);
            BOOST_REQUIRE_MESSAGE(v0->getCommoditySupply(i) == 10, "Supply set to -10");
        }

        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);


        MultiCommodityMinCostFlow::edge_t::Ptr e0(new MultiCommodityMinCostFlow::edge_t(commodities));
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        MultiCommodityMinCostFlow::edge_t::Ptr e1(new MultiCommodityMinCostFlow::edge_t(commodities));
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v2);

        e0->setCapacityUpperBound(edgeCapacityUpperBound);
        e1->setCapacityUpperBound(edgeCapacityUpperBound);

        for(size_t i = 0; i < commodities; ++i)
        {
            e0->setCommodityCapacityUpperBound(i, 10);
            BOOST_REQUIRE_MESSAGE(e0->getCommodityCapacityUpperBound(i) == 10, "Edge capacity upper bound is set to 10");
            e1->setCommodityCapacityUpperBound(i, 10);
            BOOST_REQUIRE_MESSAGE(e1->getCommodityCapacityUpperBound(i) == 10, "Edge capacity upper bound is set to 10");
        }

        graph->addEdge(e0);
        graph->addEdge(e1);

        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        uint32_t cost =  minCostFlow.run();

        std::string file("/tmp/algorithm-multicommodity-mincostflow-");
        minCostFlow.saveProblem(file + "problem");
        minCostFlow.saveSolution(file + "solution");
    }
}

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_1)
{
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance();
        uint32_t commodities = 3;
        uint32_t edgeCapacityUpperBound = 1000;

        MultiCommodityMinCostFlow::vertex_t::Ptr v0( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v1( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v2( new MultiCommodityMinCostFlow::vertex_t(commodities));

        for(size_t i = 0; i < commodities; ++i)
        {
            // Source node
            v0->setCommoditySupply(i, 50);
            // Target node
            v2->setCommoditySupply(i, -50);
        }

        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);


        MultiCommodityMinCostFlow::edge_t::Ptr e0(new MultiCommodityMinCostFlow::edge_t(commodities));
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        MultiCommodityMinCostFlow::edge_t::Ptr e1(new MultiCommodityMinCostFlow::edge_t(commodities));
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v2);

        e0->setCapacityUpperBound(edgeCapacityUpperBound);
        e1->setCapacityUpperBound(edgeCapacityUpperBound);

        for(size_t i = 0; i < commodities; ++i)
        {
            e0->setCommodityCapacityUpperBound(i, 100);
            e1->setCommodityCapacityUpperBound(i, 100);
        }

        graph->addEdge(e0);
        graph->addEdge(e1);

        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        uint32_t cost =  minCostFlow.run();

        std::string file("/tmp/algorithm-multicommodity-mincostflow-");
        minCostFlow.saveProblem(file + "problem");
        minCostFlow.saveSolution(file + "solution");
    }
}
BOOST_AUTO_TEST_SUITE_END();
