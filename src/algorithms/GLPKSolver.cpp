#include "GLPKSolver.hpp"

namespace graph_analysis {
namespace algorithms {

GLPKSolver::GLPKSolver(const std::string& problemName)
    : mpProblem(0)
{
    // define the integer program
    mpProblem = glp_create_prob();
    glp_set_prob_name(mpProblem, problemName.c_str());
}

GLPKSolver::~GLPKSolver()
{
    glp_delete_prob(mpProblem);
}

void GLPKSolver::saveProblem(const std::string& filename, LPProblemFormat format)
{
    int result = 1;
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
}

void GLPKSolver::saveSolution(const std::string& filename, LPSolutionFormat format)
{
    int result = glp_write_sol(mpProblem, filename.c_str());
    if(result != 0)
    {
        throw std::runtime_error("GLPKSolver: " + getProblemName() + " failed to save solution to '" + filename + "'");
    }
}

} // end namespace algorithms
} // end namespace graph_analysis
