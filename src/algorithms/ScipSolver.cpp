#include "ScipSolver.hpp"
#include <scip/scipdefplugins.h>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {

ScipSolver::ScipSolver()
    : mpScip(NULL)
    , mDebugSolution(true)
{
    SCIPcreate(&mpScip);
    debugSolution(mDebugSolution);
    SCIPincludeDefaultPlugins(mpScip);
   //if( SCIP_OKAY == SCIPsetBoolParam(scip, "lp/presolving", false) )
    //{
    //    std::cout << "Disabling presolving succeeded" << std::endl;
    //} else {
    //    std::cout << "Disabling presolving failed" << std::endl;
    //}

}

ScipSolver::~ScipSolver()
{
    SCIPfree(&mpScip);
}

void ScipSolver::debugSolution(bool enable)
{
    if(enable)
    {
        /* we explicitly enable the use of a debug solution for this main SCIP
         * instance */
        SCIPenableDebugSol(mpScip);
    }
}

void ScipSolver::loadProblem(const std::string& filename, LPProblemFormat format)
{
    switch(format)
    {
        case CPLEX:
            SCIPreadProb(mpScip, filename.c_str(), "lp");
            break;
        case GLPK:
            break;
        case MPS:
            break;
        default:
            break;
    }
}

void ScipSolver::saveProblem(const std::string& filename, LPProblemFormat format) const
{
    switch(format)
    {
        case CPLEX:
            break;
        case GLPK:
            break;
        case MPS:
            break;
        default:
            break;
    }
}

void ScipSolver::loadSolution(const std::string& filename, LPSolutionType format)
{
    switch(format)
    {
        case BASIC_SOLUTION:
            break;
        case IPT_SOLUTION:
            break;
        case MIP_SOLUTION:
            break;
        default:
            break;
    }
}

void ScipSolver::saveSolution(const std::string& filename, LPSolutionType format) const
{
    switch(format)
    {
        case BASIC_SOLUTION:
            break;
        case IPT_SOLUTION:
            break;
        case MIP_SOLUTION:
            break;
        default:
            LOG_INFO_S << "Saving solution: " << filename;
            SCIP_SOL* solution = 0;
            SCIPcreateCurrentSol(mpScip, &solution, NULL);
            SCIP_Bool printzero = 1;
            FILE* solutionFile = fopen(filename.c_str(),"w");
            SCIPprintSol(mpScip, solution, solutionFile, printzero);
            fclose(solutionFile);
            break;
    }
}

double ScipSolver::getObjectiveValue() const
{
    return 0.0;
}

LPSolver::Status ScipSolver::getStatus() const
{
    SCIP_STATUS status = SCIPgetStatus(mpScip);
    switch(status)
    {
        case SCIP_STATUS_OPTIMAL:
            return LPSolver::STATUS_OPTIMAL;
        case SCIP_STATUS_INFEASIBLE:
            return LPSolver::STATUS_INFEASIBLE;
        case SCIP_STATUS_UNBOUNDED:
            return LPSolver::STATUS_UNBOUNDED;
        case SCIP_STATUS_UNKNOWN:
        default:
            return LPSolver::STATUS_UNKNOWN;
    }

    return LPSolver::STATUS_UNKNOWN;
}

LPSolver::Status ScipSolver::run()
{
    SCIPsolve(mpScip);
    return getStatus();
}

LPSolver::Status ScipSolver::run(const std::string& problem, LPProblemFormat problemFormat)
{
    loadProblem(problem, problemFormat);
    return run();
}


} // end namespace algorithm
} // end namespace graph_analysis
