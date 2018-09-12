#include "ScipSolver.hpp"
#include <scip/scipdefplugins.h>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {
namespace embedded {

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

void ScipSolver::doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format)
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

void ScipSolver::saveProblem(const std::string& filename, LPSolver::ProblemFormat format) const
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

void ScipSolver::loadSolution(const std::string& filename, LPSolver::SolutionType format)
{
    switch(format)
    {
        case BASIC_SOLUTION:
        case IPT_SOLUTION:
        case MIP_SOLUTION:
        default:
            if(SCIP_OKAY != SCIPreadSol(mpScip, filename.c_str()))
            {
                throw std::runtime_error("graph_analysis::algorithms::ScipSolver: failed to read solution from '" + filename + "'");
            }
            break;
    }
}

void ScipSolver::saveSolution(const std::string& filename, LPSolver::SolutionType format) const
{
    switch(format)
    {
        case BASIC_SOLUTION:
        case IPT_SOLUTION:
        case MIP_SOLUTION:
        default:
            LOG_INFO_S << "Saving solution: " << filename;
            // Use null for current LP/pseudo solution
            SCIP_SOL* solution = NULL;
            //SCIPcreateCurrentSol(mpScip, &solution, NULL);
            SCIP_Bool printzero = 1;
            FILE* solutionFile = fopen(filename.c_str(),"w");
            if(SCIP_OKAY != SCIPprintSol(mpScip, solution, solutionFile, printzero))
            {
                fclose(solutionFile);
                throw std::runtime_error("graph_analysis::algorithms::ScipSolver: failed to write solution to '" + filename + "'");
            } else {
                fclose(solutionFile);
            }
            break;
    }
}

double ScipSolver::getObjectiveValue() const
{
    // https://stackoverflow.com/questions/24770707/get-the-objective-function-value-in-scip
    return SCIPgetPrimalbound(mpScip);
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

    // http://scip.zib.de/doc/html/FAQ.php#scipgetvarsol
    //SCIPgetSolVal(mpScip, SCIPgetBestSol(mpScip),
    return getStatus();
}

LPSolver::Status ScipSolver::run(const std::string& problem, LPSolver::ProblemFormat problemFormat, bool useCaching)
{
    loadProblem(problem, problemFormat);
    return run();
}

double ScipSolver::getVariableValue(const std::string& varName) const
{
    // http://scip.zib.de/doc/html/FAQ.php#scipgetvarsol
    SCIP_VAR* var = SCIPfindVar(mpScip, varName.c_str());
    return SCIPgetSolVal(mpScip, SCIPgetBestSol(mpScip), var);
}

double ScipSolver::getVariableValueByColumnIdx(uint32_t idx) const
{
    std::string columnName = getVariableNameByColumnIdx(idx);
    return getVariableValue(columnName);
}

} // end namespace embedded
} // end namespace lp
} // end namespace algorithm
} // end namespace graph_analysis
