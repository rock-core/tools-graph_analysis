#include "GLPKSolver.hpp"
#include <sstream>
#include <base-logging/Logging.hpp>
#include <boost/algorithm/string.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {
namespace embedded {

GLPKSolver::GLPKSolver()
    : mpProblem(NULL)
{
    mpProblem = glp_create_prob();
    glp_set_prob_name(mpProblem, "glpk-default-problem");
    glp_term_hook(GLPKSolver::printHook, NULL);
}

GLPKSolver::GLPKSolver(const std::string& problemName)
    : mpProblem(NULL)
{
    // define the integer program
    mpProblem = glp_create_prob();
    glp_set_prob_name(mpProblem, problemName.c_str());
    glp_term_hook(GLPKSolver::printHook, NULL);
}

GLPKSolver::~GLPKSolver()
{
    glp_delete_prob(mpProblem);
}

void GLPKSolver::doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format)
{
    int result = 1;
    switch(format)
    {
        case CPLEX:
            result = glp_read_lp(mpProblem, NULL, filename.c_str());
            break;
        case GLPK:
            result = glp_read_prob(mpProblem, 0, filename.c_str());
            break;
        case MPS:
            result = glp_read_mps(mpProblem, GLP_MPS_FILE, NULL, filename.c_str());
            break;
        default:
            throw std::invalid_argument("GLPKSolver: " + getProblemName() + " failed to load problem from '" + filename + "' since selected output format is unknown");
    }

    if(result != 0)
    {
        throw std::runtime_error("GLPKSolver: " + getProblemName() + " failed to load problem from '" + filename + "'");
    }

    glp_create_index(mpProblem);
}

void GLPKSolver::saveProblem(const std::string& filename, LPSolver::ProblemFormat format) const
{
    int result = -1;
    switch(format)
    {
        case CPLEX:
            result = glp_write_lp(mpProblem, NULL, filename.c_str());
            break;
        case GLPK:
            result = glp_write_prob(mpProblem, 0, filename.c_str());
            break;
        case MPS:
            result = glp_write_mps(mpProblem, GLP_MPS_FILE, NULL, filename.c_str());
            break;
        default:
            throw std::invalid_argument("GLPKSolver: " + getProblemName() + " failed to save problem to '" + filename + "' since selected output format is unknown");
    }

    if(result != 0)
    {
        throw std::runtime_error("GLPKSolver: " + getProblemName() + " failed to save problem to '" + filename + "'");
    }

    mProblemFile = filename;
}

void GLPKSolver::saveSolution(const std::string& filename, LPSolver::SolutionType format) const
{
    int result = -1;

    switch(format)
    {
        case BASIC_SOLUTION:
            result = glp_write_sol(mpProblem, filename.c_str());
            break;
        case IPT_SOLUTION:
            result = glp_write_ipt(mpProblem, filename.c_str());
            break;
        case MIP_SOLUTION:
            result = glp_write_mip(mpProblem, filename.c_str());
            break;
        default:
            throw std::invalid_argument("GLPKSolver: " + getProblemName() + " failed to save solution to '" + filename + "' since selected solution type is unknown");
    }

    if(result != 0)
    {
        throw std::runtime_error("GLPKSolver: " + getProblemName() + " failed to save solution to '" + filename + "'");
    }

    mSolutionFile = filename;
}

void GLPKSolver::loadSolution(const std::string& filename, LPSolver::SolutionType format)
{
    int result = -1;

    switch(format)
    {
        case BASIC_SOLUTION:
            result = glp_read_sol(mpProblem, filename.c_str());
            break;
        case IPT_SOLUTION:
            result = glp_read_ipt(mpProblem, filename.c_str());
            break;
        case MIP_SOLUTION:
            result = glp_read_mip(mpProblem, filename.c_str());
            break;
        default:
            throw std::invalid_argument("GLPKSolver: " + getProblemName() + " failed to read solution to '" + filename + "' since selected solution type is unknown");
    }

    if(result != 0)
    {
        throw std::runtime_error("GLPKSolver: " + getProblemName() + " failed to read solution to '" + filename + "'");
    }
}



GLPKSolver::Status GLPKSolver::translateSimplexReturnCode(int code)
{
    switch(code)
    {
        case 0:
            LOG_INFO_S << "The LP problem instance has been successfully solved. "
                << "This does not necessarily mean that the solver has found an optimal solution."
                << "It only means that the solution process was successful";
            return SOLUTION_FOUND;
        case GLP_EBADB:
            LOG_WARN_S << "Unable to start the search, because the initial basis"
                << " specified in the problem object is invalid - the number of basic (auxiliary and structural)"
                << " variables is not the same as the number of rows in the problem object";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_ESING:
            LOG_WARN_S << "Unable to start the search, because the basis matrix corresponding"
                << " to the initial basis is singular within the working precision";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_ECOND:
            LOG_WARN_S << "Unable to start the search, because the basis matrix corresponding to the "
                << "initial basis is ill-conditioned, i.e. its condition number is too large";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_EBOUND:
            LOG_WARN_S << "Unable to start the search, because some double-bounded (auxiliary or "
                << "structural) variables have incorrect bounds.";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_EFAIL:
            LOG_WARN_S << "The search was prematurely terminated due to the solver failure";
            return NO_SOLUTION_FOUND;
        case GLP_EOBJLL:
            LOG_WARN_S << "The search was prematurely terminated, because the objective function being maximized "
                << " has reached its lower limit and continues decreasing (the dual simplex only).";
            return NO_SOLUTION_FOUND;
        case GLP_EOBJUL:
            LOG_WARN_S << "The search was prematurely terminated, because the objective function being minimized "
                << " has reached its upper limit and continues increasing (the dual simplex only).";
            return NO_SOLUTION_FOUND;
        case GLP_EITLIM:
            LOG_WARN_S << "The search was prematurely terminated, because the simplex iteration lmit has been exceeded";
            return NO_SOLUTION_FOUND;
        case GLP_ETMLIM:
            LOG_WARN_S << "The search was prematurely terminated, because the time limit has been exceeded";
            return NO_SOLUTION_FOUND;
        case GLP_ENOPFS:
            LOG_WARN_S << "The LP problem instance has no primal feasible solution (only if the LP presolver is used).";
            return NO_SOLUTION_FOUND;
        case GLP_ENODFS:
            LOG_WARN_S << "The LP problem instance has no dual feasible solution (only if the LP presolve is used";
            return NO_SOLUTION_FOUND;
        default:
        {
            std::stringstream ss;
            ss << code;
            throw std::runtime_error("graph_analysis::algorithms::GLPKSolver::interpretSimplexReturnCode:"
                    " unknown return code: " + ss.str());
        }
    }
}

GLPKSolver::Status GLPKSolver::translateIntoptReturnCode(int code)
{
    switch(code)
    {
        case 0:
            LOG_INFO_S << "Solved problem sucessfully";
            return SOLUTION_FOUND;
        case GLP_EBOUND:
            LOG_WARN_S << "Unable to start the search, because some double-bounded variables have incrorrect bounds or some integer variables have non-integer (fractional) bounds.";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_EROOT:
            LOG_WARN_S << "Unable to start the search, because optimal basis for initial LP relaxation is not provided. (This error may appear only if the presolver is disabled";
            return INVALID_PROBLEM_DEFINITION;
        case GLP_ENOPFS:
            LOG_WARN_S << "Unable to start the search, because LP relaxation of the MIP problem instance has no primal feasible solution. (This error may appear only if the presolver is enabled)";
            return INVALID_PROBLEM_DEFINITION;
        default:
        {
            std::stringstream ss;
            ss << code;
            throw std::runtime_error("graph_analysis::algorithms::GLPKSolver::interpretSimplexReturnCode:"
                    " unknown return code: " + ss.str());
        }
    }
}

double GLPKSolver::getVariableValue(const std::string& varName) const
{
    int idx = glp_find_col(mpProblem, varName.c_str());
    if(idx != 0)
    {
        return glp_get_col_prim(mpProblem, idx);
    }
    throw std::invalid_argument("graph_analysis::algorithms::GLPKSolver::getVariableValue"
            "failed to identify column with name '" + varName + "'");
}

double GLPKSolver::getVariableValueByColumnIdx(uint32_t idx) const
{
    std::string columnName = LPSolver::getVariableNameByColumnIdx(idx);
    return getVariableValue(columnName);
}

GLPKSolver::Status GLPKSolver::run(const std::string& problem,
        LPSolver::ProblemFormat problemFormat, bool useCaching)
{
    GLPKSolver::Status status = GLPKSolver::STATUS_UNKNOWN;
    bool knownProblem = loadProblem(problem, problemFormat);
    try {
        if(knownProblem && useCaching)
        {
            status = loadKnownSolution(problem);
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "Error loading problem file: " << e.what();
    }

    if(status == GLPKSolver::STATUS_UNKNOWN)
    {
        // SIMPLEX
        int result = glp_simplex(mpProblem, NULL);
        status = translateSimplexReturnCode(result);
        if(useCaching)
        {
            std::string solution = saveSolutionToTempfile(BASIC_SOLUTION);
            registerSolution(problem, solution, status, BASIC_SOLUTION);
        }
    }
    return status;
}

int GLPKSolver::printHook(void* info, const char* s)
{
    std::string message(s);
    boost::trim(message);
    LOG_INFO_S << message;
    return 1;
}

} // end namespace embedded
} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
