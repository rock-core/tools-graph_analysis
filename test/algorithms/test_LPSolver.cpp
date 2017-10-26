#include <boost/test/unit_test.hpp>
#include <graph_analysis/algorithms/LPSolver.hpp>
#include "../test_utils.hpp"

using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(lp_solver)

BOOST_AUTO_TEST_CASE(scip)
{
    LPSolver::Ptr scip = LPSolver::getInstance(LPSolver::SCIP_SOLVER);
    std::string problemFilename = getRootDir() + "test/data/lp_problems/p0.lp";
    LPSolver::Status status = scip->run(problemFilename);

    BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE, "Solution status should be 'infeasible'");
}

BOOST_AUTO_TEST_CASE(glpk)
{
    LPSolver::Ptr scip = LPSolver::getInstance(LPSolver::GLPK_SOLVER);
    std::string problemFilename = getRootDir() + "test/data/lp_problems/p0.lp";
    LPSolver::Status status = scip->run(problemFilename);

    BOOST_REQUIRE_MESSAGE(status == LPSolver::SOLUTION_FOUND, "Solution status should be solution found");
}

BOOST_AUTO_TEST_CASE(glpk_with_caching)
{
    bool useCaching = true;
    LPSolver::Ptr scip = LPSolver::getInstance(LPSolver::GLPK_SOLVER);
    std::string problemFilename = getRootDir() + "test/data/lp_problems/p0.lp";

    {
        LPSolver::Status status = scip->run(problemFilename, LPSolver::CPLEX, useCaching);
        BOOST_REQUIRE_MESSAGE(status == LPSolver::SOLUTION_FOUND, "Solution status should be solution found");
    }

    for(int i = 0; i < 5; ++i)
    {
        LPSolver::Status status = scip->run(problemFilename, LPSolver::CPLEX, useCaching);
        BOOST_REQUIRE_MESSAGE(status == LPSolver::SOLUTION_FOUND, "Solution status should be solution found");
    }
}


BOOST_AUTO_TEST_SUITE_END()
