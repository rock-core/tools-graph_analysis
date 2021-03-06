#include <boost/test/unit_test.hpp>
#include <graph_analysis/algorithms/lp/Problem.hpp>
#include "../test_utils.hpp"
#include <graph_analysis/algorithms/MultiCommodityMinCostFlow.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis::algorithms::lp;

BOOST_AUTO_TEST_SUITE(lp_problem)

BOOST_AUTO_TEST_CASE(problem)
{
    Problem problem("test-problem");
    Column c0("col0", Bounds(0,10), 1.0);
    Column c1("col1", Bounds(0,10), 1.0);
    problem.addColumn(c0);
    problem.addColumn(c1);

    Row row0("row0", Bounds(0,10));
    {
        MatrixEntry a("col0",2.0);
        MatrixEntry b("col1",3.0);
        row0.entries.push_back(a);
        row0.entries.push_back(b);
    }
    problem.addRow(row0);

    Row row1("row1", Bounds(0,10));
    {
        MatrixEntry a("col0",7.0);
        MatrixEntry b("col1",8.0);
        row1.entries.push_back(a);
        row1.entries.push_back(b);
    }
    problem.addRow(row1);

    BOOST_TEST_MESSAGE(problem.toString());

    std::string filename = problem.saveProblemToTempfile();
    LPSolver::Ptr solver = LPSolver::getInstance(LPSolver::GLPK_SOLVER);
    LPSolver::Status status = solver->run(filename);
    BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL, "Optimal status"
            "expected was " << LPSolver::StatusTxt[status]);
}

BOOST_AUTO_TEST_CASE(mmcf)
{
    using namespace graph_analysis;
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

    MultiCommodityMinCostFlow minCostFlow(graph, commodities, LPSolver::GLPK_SOLVER);
    LPSolver::Status status = minCostFlow.solve();
    BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL, "Optimal status"
            "expected was " << LPSolver::StatusTxt[status]);
}

BOOST_AUTO_TEST_SUITE_END()
