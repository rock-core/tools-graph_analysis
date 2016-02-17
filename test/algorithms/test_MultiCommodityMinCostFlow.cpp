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

        std::string file("/tmp/algorithm-multicommodity-mincostflow-0.");
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

        std::string file("/tmp/algorithm-multicommodity-mincostflow-1.");
        minCostFlow.saveProblem(file + "problem");
        minCostFlow.saveSolution(file + "solution");
    }
}

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_2)
{
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance();
        uint32_t commodities = 3;
        uint32_t edgeCapacityUpperBound = 60;

        MultiCommodityMinCostFlow::vertex_t::Ptr v0( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v1( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v2( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v3( new MultiCommodityMinCostFlow::vertex_t(commodities));

        for(size_t i = 0; i < commodities; ++i)
        {
            // Source node
            v0->setCommoditySupply(i, 50);
            // Target node
            v3->setCommoditySupply(i, -50);
        }

        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);
        graph->addVertex(v3);

        // Edges
        MultiCommodityMinCostFlow::edge_t::Ptr e0(new MultiCommodityMinCostFlow::edge_t(commodities));
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);
        e0->setCapacityUpperBound(edgeCapacityUpperBound);

        MultiCommodityMinCostFlow::edge_t::Ptr e1(new MultiCommodityMinCostFlow::edge_t(commodities));
        e1->setSourceVertex(v0);
        e1->setTargetVertex(v2);
        e1->setCapacityUpperBound(edgeCapacityUpperBound);

        MultiCommodityMinCostFlow::edge_t::Ptr e2(new MultiCommodityMinCostFlow::edge_t(commodities));
        e2->setSourceVertex(v1);
        e2->setTargetVertex(v3);
        e2->setCapacityUpperBound(edgeCapacityUpperBound);

        MultiCommodityMinCostFlow::edge_t::Ptr e3(new MultiCommodityMinCostFlow::edge_t(commodities));
        e3->setSourceVertex(v2);
        e3->setTargetVertex(v3);
        e3->setCapacityUpperBound(edgeCapacityUpperBound);

        MultiCommodityMinCostFlow::edge_t::Ptr e4(new MultiCommodityMinCostFlow::edge_t(commodities));
        e4->setSourceVertex(v0);
        e4->setTargetVertex(v3);
        e4->setCapacityUpperBound(edgeCapacityUpperBound);
        e4->setCommodityCost(0,100);
        e4->setCommodityCost(1,100);
        e4->setCommodityCost(2,100);


        for(size_t i = 0; i < commodities; ++i)
        {
            e0->setCommodityCapacityUpperBound(i, 20);
            e1->setCommodityCapacityUpperBound(i, 20);
            e2->setCommodityCapacityUpperBound(i, 20);
            e3->setCommodityCapacityUpperBound(i, 20);
            e4->setCommodityCapacityUpperBound(i, 20);
        }

        graph->addEdge(e0);
        graph->addEdge(e1);
        graph->addEdge(e2);
        graph->addEdge(e3);
        graph->addEdge(e4);


        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        uint32_t cost = minCostFlow.run();

        std::string file("/tmp/algorithm-multicommodity-mincostflow-2.");
        minCostFlow.saveProblem(file + "problem");
        minCostFlow.saveSolution(file + "solution");

        minCostFlow.storeResult();
        EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
        while(edgeIt->next())
        {
            MultiCommodityMinCostFlow::edge_t::Ptr edge = boost::dynamic_pointer_cast<MultiCommodityMinCostFlow::edge_t>( edgeIt->current() );
            if(edge)
            {
                std::stringstream ss;
                ss << "Flows on: " << edge->toString() << ": id " << graph->getEdgeId(edge) << std::endl;

                for(size_t i = 0; i < commodities; ++i)
                {
                    ss << "    commodity #" << i << ": " << edge->getCommodityFlow(i) << std::endl;
                }
                BOOST_TEST_MESSAGE(ss.str());
            }
        }
        BOOST_TEST_MESSAGE("Final objective value: " << minCostFlow.getObjectiveValue());
    }
}
BOOST_AUTO_TEST_SUITE_END();
