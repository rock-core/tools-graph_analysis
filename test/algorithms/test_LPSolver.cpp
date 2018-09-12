#include <boost/test/unit_test.hpp>
#include <graph_analysis/SharedPtr.hpp>
#include <graph_analysis/algorithms/LPSolver.hpp>
#include <graph_analysis/algorithms/lp/CommandlineSolver.hpp>
#include "../test_utils.hpp"

using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(lp_solver)

BOOST_AUTO_TEST_CASE(solvers)
{
    for(int i = (int) LPSolver::UNKNOWN_LP_SOLVER + 1; i < (int) LPSolver::LP_SOLVER_TYPE_END; ++i)
    {
        LPSolver::Type type = (LPSolver::Type) i;
        try {
            LPSolver::Ptr scip = LPSolver::getInstance(type);
            std::string problemFilename = getRootDir() + "test/data/lp_problems/p0.lp";
            LPSolver::Status status = scip->run(problemFilename);

            switch(type)
            {
                case LPSolver::CBC_SOLVER:
                case LPSolver::GLPK_SOLVER:
                case LPSolver::SCIP_SOLVER:
                    BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE,
                            "Solution status for '" + LPSolver::TypeTxt[type] + "' should be 'infeasible' was " << LPSolver::StatusTxt[status]);
                    break;
                default:
                    break;
            }
        } catch(const std::invalid_argument& e)
        {
            BOOST_TEST_MESSAGE("Solver not implemented? -- " << e.what());
        }
    }
}

BOOST_AUTO_TEST_CASE(scip)
{
    using namespace graph_analysis;
    LPSolver::Ptr scip = LPSolver::getInstance(LPSolver::SCIP_SOLVER);
    shared_ptr<lp::CommandlineSolver> scipCmd = dynamic_pointer_cast<lp::CommandlineSolver>(scip);

    {
        std::string problemFilename = getRootDir() + "test/data/lp_problems/scip-infeasible-solution.lp";
        lp::Solution solution = scipCmd->readBasicSolution(problemFilename);
        LPSolver::Status status = solution.getStatus();

        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE,
                        "Solution status for SCIP solver should be 'infeasible', was " << LPSolver::StatusTxt[status]);
    }

    {
        std::string problemFilename = getRootDir() + "test/data/lp_problems/scip-feasible-solution.lp";
        lp::Solution solution = scipCmd->readBasicSolution(problemFilename);
        LPSolver::Status status = solution.getStatus();

        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL,
                        "Solution status for SCIP solver should be 'optimal', was " << LPSolver::StatusTxt[status]);
        std::map<std::string, double> solutionValues =
        {
            {"x1",1.0}
            , {"x2",0.0}
            , {"x6",1.0}
            , {"x8",1.0}
            , {"x14",1.0}
            , {"x16",1.0}
            , {"x18",1.0}
            , {"x21",1.0}
        };
        for(const std::pair<std::string, double>& p : solutionValues)
        {
            double columnValue = solution.getColumnValue(p.first);
            BOOST_REQUIRE_MESSAGE(p.second == columnValue,
                    "Column " << p.first << ": " << columnValue <<
                    ", expected: " << p.second);
        }

        BOOST_REQUIRE_MESSAGE(solution.getObjectiveValue() == 6.0, "Objective"
                "value was " << solution.getObjectiveValue() << ", expected " <<
                solution.getObjectiveValue());
    }
}

BOOST_AUTO_TEST_CASE(clp)
{
    using namespace graph_analysis;
    LPSolver::Ptr solver = LPSolver::getInstance(LPSolver::CLP_SOLVER);
    shared_ptr<lp::CommandlineSolver> solverCmd =
        dynamic_pointer_cast<lp::CommandlineSolver>(solver);

    {
        std::string problemFilename = getRootDir() + "test/data/lp_problems/clp-infeasible-solution.lp";
        lp::Solution solution = solverCmd->readBasicSolution(problemFilename);
        LPSolver::Status status = solution.getStatus();

        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_INFEASIBLE,
                        "Solution status for CLP solver should be 'infeasible', was " << LPSolver::StatusTxt[status]);
    }

    {
        std::string problemFilename = getRootDir() + "test/data/lp_problems/clp-feasible-solution.lp";
        lp::Solution solution = solverCmd->readBasicSolution(problemFilename);
        LPSolver::Status status = solution.getStatus();

        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL,
                        "Solution status for CLP solver should be 'optimal', was " << LPSolver::StatusTxt[status]);
        std::map<std::string, double> solutionValues =
        {
            {"x1",1.0}
            , {"x2",1.0}
            , {"x4",1.0}
            , {"x9",1.0}
            , {"x11",1.0}
            , {"x19",1.0}
            , {"x22",1.0}
        };
        for(const std::pair<std::string, double>& p : solutionValues)
        {
            double columnValue = solution.getColumnValue(p.first);
            BOOST_REQUIRE_MESSAGE(p.second == columnValue,
                    "Column " << p.first << ": " << columnValue <<
                    ", expected: " << p.second);
        }

        BOOST_REQUIRE_MESSAGE(solution.getObjectiveValue() == 6.0, "Objective"
                "value was " << solution.getObjectiveValue() << ", expected " <<
                solution.getObjectiveValue());
    }
}

BOOST_AUTO_TEST_CASE(glpk_with_caching)
{
    bool useCaching = true;
    LPSolver::Ptr solver = LPSolver::getInstance(LPSolver::GLPK_SOLVER);
    std::string problemFilename = getRootDir() + "test/data/lp_problems/feasible-problem.lp";

    {
        LPSolver::Status status = solver->run(problemFilename, LPSolver::CPLEX, useCaching);
        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL, "Solution "
                "status should be 'optimal', was " << LPSolver::StatusTxt[status]);
    }

    for(int i = 0; i < 5; ++i)
    {
        LPSolver::Status status = solver->run(problemFilename, LPSolver::CPLEX, useCaching);
        BOOST_REQUIRE_MESSAGE(status == LPSolver::STATUS_OPTIMAL, "Solution "
                "status should be 'optimal', was " << LPSolver::StatusTxt[status]);
    }
}


BOOST_AUTO_TEST_SUITE_END()
