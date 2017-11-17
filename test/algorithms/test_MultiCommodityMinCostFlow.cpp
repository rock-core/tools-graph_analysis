#include <boost/test/unit_test.hpp>
#include "../test_utils.hpp"

#include <graph_analysis/algorithms/MultiCommodityMinCostFlow.hpp>
#include <graph_analysis/GraphIO.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms_min_cost_flow)

/**
 * v0  -- e0 --> v1 -- e1 --> v2
 *
 * Route a number of commodities across this single line
 *
 */
BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_0)
{
    std::string savedProblem;
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
        BOOST_REQUIRE_MESSAGE(v0->getCommoditySupply(i) == 10, "Source node: supply set to 10");
        // Target node
        v2->setCommoditySupply(i, -10);
        BOOST_REQUIRE_MESSAGE(v2->getCommoditySupply(i) == -10, "Target node: supply set to -10 (negative values meaning demand)");
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
        BOOST_REQUIRE_MESSAGE(e0->getCommodityCapacityUpperBound(i) == 10, "First edge capacity upper bound is set to 10");
        e1->setCommodityCapacityUpperBound(i, 10);
        BOOST_REQUIRE_MESSAGE(e1->getCommodityCapacityUpperBound(i) == 10, "Second edge capacity upper bound is set to 10");
    }

    graph->addEdge(e0);
    graph->addEdge(e1);

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_0");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            uint32_t cost =  minCostFlow.solve(prefixPath);
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should have no flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_0.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_0-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }

        {
            MultiCommodityMinCostFlow minCostFlow = MultiCommodityMinCostFlow::fromFile(savedProblem, representation::UNKNOWN, solverType);
            LPSolver::Status status = minCostFlow.solve();
            BOOST_TEST_MESSAGE("Cost after reloading are: " << minCostFlow.getCost());
        }
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

        std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_1");
        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        minCostFlow.solve(prefixPath);
        BOOST_TEST_MESSAGE("Cost are: " << minCostFlow.getCost());

        minCostFlow.save(prefixPath, representation::GRAPHVIZ);

        std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
        BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should have no flaws, but has " << ConstraintViolation::toString(flaws) );

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

        std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_2");

        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        minCostFlow.solve(prefixPath);
        BOOST_TEST_MESSAGE("Cost are: " << minCostFlow.getCost());

        minCostFlow.save(prefixPath, representation::GRAPHVIZ);

        EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
        while(edgeIt->next())
        {
            MultiCommodityMinCostFlow::edge_t::Ptr edge = dynamic_pointer_cast<MultiCommodityMinCostFlow::edge_t>( edgeIt->current() );
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
        BOOST_TEST_MESSAGE("Final objective value: " << minCostFlow.getCost());

        std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
        BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should have no flaws, but has " << ConstraintViolation::toString(flaws) );
    }

}

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_3)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    uint32_t commodities = 2;

    /**
     *  +     O     O     O
     *  |     |     |     |
     *  O     O     O     O
     *  |     |     |     |
     *  O     O     O     O
     *  |     |     |     |
     *  O     O     O     -
     */

    std::vector<MultiCommodityMinCostFlow::vertex_t::Ptr> vertices;
    for(int i = 0; i < 16; ++i)
    {
        MultiCommodityMinCostFlow::vertex_t::Ptr v( new MultiCommodityMinCostFlow::vertex_t(commodities));
        vertices.push_back(v);
        graph->addVertex(v);
        std::stringstream ss;
        ss << i;
        v->setLabel(ss.str());
    }

    for(size_t i = 0; i < commodities; ++i)
    {
        // Source node
        vertices.front()->setCommoditySupply(i, 10);
        // Target node
        vertices.back()->setCommoditySupply(i, -10);
    }

    // Edges
    int colSize = 4;
    int rowSize = 4;
    for(int col = 0; col < colSize; ++col)
    {
        for(int row = 0; row < rowSize; ++row)
        {
            if(row < rowSize -1)
            {
                MultiCommodityMinCostFlow::edge_t::Ptr e0(new MultiCommodityMinCostFlow::edge_t(commodities));
                e0->setSourceVertex(vertices[colSize*row + col]);
                e0->setTargetVertex(vertices[colSize*(row+1) + col]);
                e0->setCapacityUpperBound(1000); // unlimited for direct connection
                graph->addEdge(e0);
                for(size_t i = 0; i < commodities; ++i)
                {
                    e0->setCommodityCapacityUpperBound(i,1000);
                    e0->setCommodityCost(i,1);
                }
            }
        }
    }

    MultiCommodityMinCostFlow::edge_t::Ptr e1(new MultiCommodityMinCostFlow::edge_t(commodities));
    e1->setSourceVertex(vertices.front());
    e1->setTargetVertex(vertices[1*colSize + colSize -1]);
    e1->setCapacityUpperBound(8);
    graph->addEdge(e1);

    MultiCommodityMinCostFlow::edge_t::Ptr e2(new MultiCommodityMinCostFlow::edge_t(commodities));
    e2->setSourceVertex(vertices.front());
    e2->setTargetVertex(vertices[1*colSize + colSize -1]);
    e2->setCapacityUpperBound(10);
    graph->addEdge(e2);

    MultiCommodityMinCostFlow::edge_t::Ptr e3(new MultiCommodityMinCostFlow::edge_t(commodities));
    e3->setSourceVertex(vertices[1*colSize + 0]);
    e3->setTargetVertex(vertices[2*colSize + colSize - 1]);
    e3->setCapacityUpperBound(2);
    graph->addEdge(e3);

    for(size_t i = 0; i < commodities; ++i)
    {
        e1->setCommodityCapacityUpperBound(i, 20);
        e2->setCommodityCapacityUpperBound(i, 20);
        e3->setCommodityCapacityUpperBound(i, 20);

        e1->setCommodityCost(i, 10);
        e2->setCommodityCost(i, 10);
        e3->setCommodityCost(i, 10);
    }

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
            LPSolver::Type solverType = (LPSolver::Type) i;

            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_3");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            minCostFlow.solve(prefixPath);
            BOOST_TEST_MESSAGE("Cost are: " << minCostFlow.getCost());

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
            while(edgeIt->next())
            {
                MultiCommodityMinCostFlow::edge_t::Ptr edge = dynamic_pointer_cast<MultiCommodityMinCostFlow::edge_t>( edgeIt->current() );
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
            BOOST_TEST_MESSAGE("Final objective value: " << minCostFlow.getCost());

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should have no flaws, but has " << ConstraintViolation::toString(flaws) << " solver type: " << LPSolver::TypeTxt[solverType] );
    }
}

BOOST_AUTO_TEST_CASE(problem_from_file_0)
{
    std::string filename = getRootDir() + "/test/data/multicommodity-min-cost-flow.gexf";
    MultiCommodityMinCostFlow minCostFlow = MultiCommodityMinCostFlow::fromFile(filename);
    minCostFlow.solve();
    BOOST_TEST_MESSAGE("Resulting cost are: " << minCostFlow.getCost());

    std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
    BOOST_REQUIRE_MESSAGE(!flaws.empty(), "Solution will have flaws, but has " << ConstraintViolation::toString(flaws) );

}

/**
 * v0  -- e0 --> v1 -- e1 --> v2
 * 10  -->                --> -10
 * Route a number of commodities across this single line but with subcapacity
 * constraints
 *
 */
BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_4)
{
    std::string savedProblem;
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
        BOOST_REQUIRE_MESSAGE(v0->getCommoditySupply(i) == 10, "Source node: supply set to 10");
        // Target node
        v2->setCommoditySupply(i, -10);
        BOOST_REQUIRE_MESSAGE(v2->getCommoditySupply(i) == -10, "Target node: supply set to -10 (negative values meaning demand)");
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
    {
        MultiCommodityEdge::CommoditySet commoditiesGroup;
        commoditiesGroup.insert(0);
        commoditiesGroup.insert(1);
        e0->setSubCapacityUpperBound(commoditiesGroup, 1);
    }
    e1->setCapacityUpperBound(edgeCapacityUpperBound);

    for(size_t i = 0; i < commodities; ++i)
    {
        e0->setCommodityCapacityUpperBound(i, 10);
        BOOST_REQUIRE_MESSAGE(e0->getCommodityCapacityUpperBound(i) == 10, "First edge capacity upper bound is set to 10");
        e1->setCommodityCapacityUpperBound(i, 10);
        BOOST_REQUIRE_MESSAGE(e1->getCommodityCapacityUpperBound(i) == 10, "Second edge capacity upper bound is set to 10");
    }

    graph->addEdge(e0);
    graph->addEdge(e1);

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_4");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            uint32_t cost =  minCostFlow.solve(prefixPath);
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(!flaws.empty(), "Solution should have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_4.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_4-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}

/**
 * v0  -- e0 --> v1 -- e1 --> v2
 * 10  -->                --> -10
 * Route a number of commodities across this single line but with subcapacity
 * constraints
 *
 */
BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_5)
{
    std::string savedProblem;
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    uint32_t commodities = 3;
    uint32_t edgeCapacityUpperBound = 30;

    MultiCommodityMinCostFlow::vertex_t::Ptr v0( new MultiCommodityMinCostFlow::vertex_t(commodities));
    MultiCommodityMinCostFlow::vertex_t::Ptr v1( new MultiCommodityMinCostFlow::vertex_t(commodities));
    MultiCommodityMinCostFlow::vertex_t::Ptr v2( new MultiCommodityMinCostFlow::vertex_t(commodities));

    for(size_t i = 0; i < commodities; ++i)
    {
        // Source node
        v0->setCommoditySupply(i, 5);
        BOOST_REQUIRE_MESSAGE(v0->getCommoditySupply(i) == 5, "Source node: supply set to 5");
        // Target node
        v2->setCommoditySupply(i, -5);
        BOOST_REQUIRE_MESSAGE(v2->getCommoditySupply(i) == -5, "Target node: supply set to -5 (negative values meaning demand)");
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
    {
        MultiCommodityEdge::CommoditySet commoditiesGroup;
        commoditiesGroup.insert(0);
        commoditiesGroup.insert(1);
        e0->setSubCapacityUpperBound(commoditiesGroup, 10);
    }
    e1->setCapacityUpperBound(edgeCapacityUpperBound);

    for(size_t i = 0; i < commodities; ++i)
    {
        e0->setCommodityCapacityUpperBound(i, 10);
        BOOST_REQUIRE_MESSAGE(e0->getCommodityCapacityUpperBound(i) == 10, "First edge capacity upper bound is set to 10");
        e1->setCommodityCapacityUpperBound(i, 10);
        BOOST_REQUIRE_MESSAGE(e1->getCommodityCapacityUpperBound(i) == 10, "Second edge capacity upper bound is set to 10");
    }

    graph->addEdge(e0);
    graph->addEdge(e1);

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_5");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            uint32_t cost =  minCostFlow.solve(prefixPath);
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should not have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_5.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_5-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}
BOOST_AUTO_TEST_SUITE_END();
