#include <boost/test/unit_test.hpp>
#include <graph_analysis/algorithms/LPSolver.hpp>
#include "../test_utils.hpp"

using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(lp_solver)

BOOST_AUTO_TEST_CASE(solvers)
{
    for(int i = (int) LPSolver::GLPK_SOLVER; i < (int) LPSolver::LP_SOLVER_TYPE_END; ++i)
    {
        LPSolver::Type type = (LPSolver::Type) i;
        LPSolver::Ptr scip = LPSolver::getInstance(type);
        std::string problemFilename = getRootDir() + "test/data/lp_problems/p0.lp";
        LPSolver::Status status = scip->run(problemFilename);

        switch(type)
        {
            case LPSolver::GLPK_SOLVER:
                BOOST_REQUIRE_MESSAGE(status == LPSolver::SOLUTION_FOUND, "Solution status '" + LPSolver::TypeTxt[type] + "' should be solution found");
                break;
            case LPSolver::SCIP_SOLVER:
                BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE, "Solution status for '" + LPSolver::TypeTxt[type] + "' should be 'infeasible'");
                break;
            case LPSolver::SOPLEX_SOLVER:
                BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE,
                        "Solution status for '" + LPSolver::TypeTxt[type] + "' should be 'infeasible' was " << LPSolver::StatusTxt[status]);
                break;
            default:
                break;
        }
    }
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
