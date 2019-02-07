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
    e1->setSourceVertex(vertices.front()); // row 0
    e1->setTargetVertex(vertices[colSize*1 + 1]); // row 1 x 1
    e1->setCapacityUpperBound(20);
    graph->addEdge(e1);

    MultiCommodityMinCostFlow::edge_t::Ptr e2(new MultiCommodityMinCostFlow::edge_t(commodities));
    e2->setSourceVertex(vertices[colSize*1 + 1]);
    e2->setTargetVertex(vertices[colSize*2 + 2]);
    e2->setCapacityUpperBound(20);
    graph->addEdge(e2);

    MultiCommodityMinCostFlow::edge_t::Ptr e3(new MultiCommodityMinCostFlow::edge_t(commodities));
    e3->setSourceVertex(vertices[2*colSize + 2]);
    e3->setTargetVertex(vertices[3*colSize + 3]);
    e3->setCapacityUpperBound(20);
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

BOOST_AUTO_TEST_CASE(problem_serialization)
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

    std::string filename = getRootDir() + "test/data/multicommodity-min-cost-flow.gexf";
    MultiCommodityMinCostFlow minCostFlowOrig(graph, commodities);
    minCostFlowOrig.save(filename);

    MultiCommodityMinCostFlow minCostFlow = MultiCommodityMinCostFlow::fromFile(filename);
    minCostFlow.solve();

    std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
    BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution will have no flaws");
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

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_6)
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

        // Set commodity min transflow
        v1->setCommodityMinTransFlow(i,5);
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
        e1->setCommodityCapacityUpperBound(i, 10);
    }

    graph->addEdge(e0);
    graph->addEdge(e1);

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_6");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            LPSolver::Status status =  minCostFlow.solve(prefixPath);

            BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL || status == LPSolver::SOLUTION_FOUND, "Solver status should be: optimal was " << LPSolver::StatusTxt[status]);

            double cost = minCostFlow.getCost();
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should not have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_6.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_6-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}

BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_7)
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

        // Set commodity min transflow
        v1->setCommodityMinTransFlow(i,5);
    }
    v1->setCommoditiesInFlowBounds({0,1,2}, std::pair<uint32_t,uint32_t>(0,15));

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
        e1->setCommodityCapacityUpperBound(i, 10);
    }

    graph->addEdge(e0);
    graph->addEdge(e1);

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_7");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            LPSolver::Status status =  minCostFlow.solve(prefixPath);

            BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL || status == LPSolver::SOLUTION_FOUND, "Solver status should be: optimal was " << LPSolver::StatusTxt[status]);

            double cost = minCostFlow.getCost();
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should not have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_7.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_7-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}

// Test setting of transflow constraints
BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_8)
{
    std::string savedProblem;
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    uint32_t commodities = 1;

    std::map<std::string, MultiCommodityMinCostFlow::vertex_t::Ptr> vertices;
    std::vector<std::string> labels = { "start","end" };
    std::vector<std::string> locations = { "l0","l1","l2" };
    std::vector<std::string> timepoints = { "t0","t1","t2","t5","t6","t7" };

    for(const std::string& l : labels)
    {
        MultiCommodityMinCostFlow::vertex_t::Ptr v =
            make_shared<MultiCommodityMinCostFlow::vertex_t>(commodities,l);

        vertices[l] = v;
        graph->addVertex(v);
    }

    for(const std::string& l : locations)
    {
        for(const std::string& t: timepoints)
        {
            std::string label = l +"-" + t;
            MultiCommodityMinCostFlow::vertex_t::Ptr v =
                make_shared<MultiCommodityMinCostFlow::vertex_t>(commodities,label);

            vertices[label] = v;
            graph->addVertex(v);
            BOOST_TEST_MESSAGE("Added: " << v->toString());
        }
    }
    // Depot demand/supply
    vertices["start"]->setCommoditySupply(0,1);
    vertices["end"]->setCommoditySupply(0,-1);

    // Transflows for paths
    vertices["l1-t0"]->setCommodityMinTransFlow(0,1);
    vertices["l1-t1"]->setCommodityMinTransFlow(0,1);
    vertices["l2-t6"]->setCommodityMinTransFlow(0,1);
    vertices["l2-t7"]->setCommodityMinTransFlow(0,1);

    // Edges
    // Start at 'start'
    // commodity can only go to l1-t0
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,0);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l0-t0"]);
        graph->addEdge(e);
    }
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,1);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l1-t0"]);
        graph->addEdge(e);
    }
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,0);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l2-t0"]);
        graph->addEdge(e);
    }

    // Enable all local transitions
    for(size_t i = 0; i < timepoints.size()-1; ++i)
    {
        for(const std::string& l : locations)
        {
            MultiCommodityMinCostFlow::edge_t::Ptr e =
                make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
            e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
            e->setCommodityCapacityUpperBound(0,std::numeric_limits<uint32_t>::max());
            e->setCommodityCost(0,1);

            std::string fromLabel = l + "-" + timepoints[i];
            std::string toLabel = l + "-" + timepoints[i+1];
            BOOST_TEST_MESSAGE("FROM: " << fromLabel << " to " << toLabel);
            e->setSourceVertex(vertices[fromLabel]);
            e->setTargetVertex(vertices[toLabel]);
            graph->addEdge(e);
        }
    }

    // Additional transitions
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,4);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["l1-t1"]);
        e->setTargetVertex(vertices["l2-t2"]);
        graph->addEdge(e);
    }
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,4);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["l2-t6"]);
        e->setTargetVertex(vertices["l1-t7"]);
        graph->addEdge(e);
    }

    // to final depot
    // Enable all local transitions
    for(const std::string& l : locations)
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,std::numeric_limits<uint32_t>::max());
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices[l + "-" + timepoints.back()]);
        e->setTargetVertex(vertices["end"]);
        graph->addEdge(e);
    }

    for(int i = (int) LPSolver::UNKNOWN_LP_SOLVER + 1; i < (int) LPSolver::LP_SOLVER_TYPE_END; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
#ifndef WITH_SCIP
            if(solverType == LPSolver::SCIP_SOLVER || solverType == LPSolver::SOPLEX_SOLVER
                    || solverType == LPSolver::SOPLEX_SOLVER_EMBEDDED
                    || solverType == LPSolver::SCIP_SOLVER_EMBEDDED)
            {
                continue;
            }
#endif
#ifndef WITH_GLPK
            if(solverType == LPSolver::GLPK_SOLVER || solverType ==
                    LPSolver::GLPK_SOLVER_EMBEDDED)
            {
                continue;
            }
#endif

        {
            std::string prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_8");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            LPSolver::Status status =  minCostFlow.solve(prefixPath);

            BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL || status == LPSolver::SOLUTION_FOUND, "Solver status should be: optimal was " << LPSolver::StatusTxt[status]);

            double cost = minCostFlow.getCost();
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should not have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_8.dot", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_8-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}

// Test setting of transflow constraints
BOOST_AUTO_TEST_CASE(multi_commodity_min_cost_flow_9)
{
    std::string savedProblem;
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    uint32_t commodities = 1;

    std::map<std::string, MultiCommodityMinCostFlow::vertex_t::Ptr> vertices;
    std::vector<std::string> labels = { "start","end" };
    std::vector<std::string> locations = { "l0","l1","l2" };
    std::vector<std::string> timepoints = { "t0","t1","t1.5", "t2","t3","t4","t4.5","t5","t6","t7" };

    for(const std::string& l : labels)
    {
        MultiCommodityMinCostFlow::vertex_t::Ptr v =
            make_shared<MultiCommodityMinCostFlow::vertex_t>(commodities,l);

        vertices[l] = v;
        graph->addVertex(v);
    }

    for(const std::string& l : locations)
    {
        for(const std::string& t: timepoints)
        {
            std::string label = l +"-" + t;
            MultiCommodityMinCostFlow::vertex_t::Ptr v =
                make_shared<MultiCommodityMinCostFlow::vertex_t>(commodities,label);

            vertices[label] = v;
            graph->addVertex(v);
            BOOST_TEST_MESSAGE("Added: " << v->toString());
        }
    }
    // Depot demand/supply
    vertices["start"]->setCommoditySupply(0,1);
    vertices["end"]->setCommoditySupply(0,-1);

    // Transflows for paths
    vertices["l1-t0"]->setCommodityMinTransFlow(0,1);
    vertices["l1-t1"]->setCommodityMinTransFlow(0,1);
    vertices["l0-t3"]->setCommodityMinTransFlow(0,1);
    vertices["l0-t4"]->setCommodityMinTransFlow(0,1);
    vertices["l1-t6"]->setCommodityMinTransFlow(0,1);
    vertices["l1-t7"]->setCommodityMinTransFlow(0,1);

    // Edges
    // Start at 'start'
    // commodity can only go to l0-t0
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,0);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l0-t0"]);
        graph->addEdge(e);
    }
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,1);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l1-t0"]);
        graph->addEdge(e);
    }
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,0);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices["start"]);
        e->setTargetVertex(vertices["l2-t0"]);
        graph->addEdge(e);
    }

    // Enable all local transitions
    for(size_t i = 0; i < timepoints.size()-1; ++i)
    {
        for(const std::string& l : locations)
        {
            MultiCommodityMinCostFlow::edge_t::Ptr e =
                make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
            e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
            e->setCommodityCapacityUpperBound(0,std::numeric_limits<uint32_t>::max());
            e->setCommodityCost(0,1);

            std::string fromLabel = l + "-" + timepoints[i];
            std::string toLabel = l + "-" + timepoints[i+1];
            BOOST_TEST_MESSAGE("FROM: " << fromLabel << " to " << toLabel);
            e->setSourceVertex(vertices[fromLabel]);
            e->setTargetVertex(vertices[toLabel]);
            graph->addEdge(e);
        }
    }

    // Additional transitions
    // from - to tuples
    std::vector< std::vector<std::string> > addTransitions =
    { { "l1-t1"  ,"l0-t1.5" },
      { "l0-t1.5", "l2-t2" },
      { "l2-t5"  , "l0-t6" },
      { "l0-t5"  , "l1-t6" } };
    for(const std::vector<std::string> link : addTransitions)
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,4);
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices[ link[0] ]);
        e->setTargetVertex(vertices[ link[1] ]);
        graph->addEdge(e);
    }

    // to final depot
    // Enable all local transitions
    for(const std::string& l : locations)
    {
        MultiCommodityMinCostFlow::edge_t::Ptr e =
            make_shared<MultiCommodityMinCostFlow::edge_t>(commodities);
        e->setCapacityUpperBound(std::numeric_limits<uint32_t>::max());
        e->setCommodityCapacityUpperBound(0,std::numeric_limits<uint32_t>::max());
        e->setCommodityCost(0,1);
        e->setSourceVertex(vertices[l + "-" + timepoints.back()]);
        e->setTargetVertex(vertices["end"]);
        graph->addEdge(e);
    }

    for(int i = LPSolver::GLPK_SOLVER; i != (int) LPSolver::SOPLEX_SOLVER; ++i)
    {
        LPSolver::Type solverType = (LPSolver::Type) i;
        {
            std::string
                prefixPath("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_9");
            MultiCommodityMinCostFlow minCostFlow(graph, commodities, solverType);
            minCostFlow.save(prefixPath, representation::GRAPHVIZ);
            LPSolver::Status status =  minCostFlow.solve(prefixPath);

            BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL || status == LPSolver::SOLUTION_FOUND, "Solver status should be: optimal was " << LPSolver::StatusTxt[status]);

            double cost = minCostFlow.getCost();
            BOOST_TEST_MESSAGE("Cost are: " << cost);

            minCostFlow.save(prefixPath, representation::GRAPHVIZ);

            std::vector<ConstraintViolation> flaws = minCostFlow.validateInflow();
            BOOST_REQUIRE_MESSAGE(flaws.empty(), "Solution should not have flaws, but has " << ConstraintViolation::toString(flaws) );

            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_9.dot", graph);
            io::GraphIO::write("/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_9.gexf", graph);

            savedProblem = "/tmp/graph_analysis-test-algorithms-multi_commodity_min_cost_flow_9-problem-export.gexf";
            minCostFlow.save(savedProblem);
        }
    }
}
BOOST_AUTO_TEST_SUITE_END();
