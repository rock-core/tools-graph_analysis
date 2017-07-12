#ifndef GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP

#include <glpk.h>
#include <string>
#include <stdexcept>

namespace graph_analysis {
namespace algorithms {

enum LPProblemFormat { UNKNOWN_PROBLEM_FORMAT = 0, CPLEX, GLPK, MPS };

/// Solution types: basic, IPT = interior point, MIP = mixed integer
enum LPSolutionType { UNKNOWN_SOLUTION_TYPE = 0, BASIC_SOLUTION, IPT_SOLUTION, MIP_SOLUTION };

/**
 * General class to interface with the Gnu Linear Programming Kit
 * \see http://kam.mff.cuni.cz/~elias/glpk.pdf
 */
class GLPKSolver
{
public:
    enum Status { INVALID_PROBLEM_DEFINITION, NO_SOLUTION_FOUND, SOLUTION_FOUND };

    GLPKSolver(const std::string& problemName);
    virtual ~GLPKSolver();

    std::string getProblemName() const { return std::string( glp_get_prob_name(mpProblem) ) ; }

    void loadProblem(const std::string& filename, LPProblemFormat format = CPLEX);

    void saveProblem(const std::string& filename, LPProblemFormat format = CPLEX);

    void saveSolution(const std::string& filename, LPSolutionType format = BASIC_SOLUTION);

    double getObjectiveValue() const { return glp_get_obj_val(mpProblem); }

    virtual Status run() { throw std::runtime_error("graph_analysis::algorithms::GLPKSolver::run not implemented"); }

protected:
    glp_prob* mpProblem;

    Status translateSimplexReturnCode(int code);
    Status translateIntoptReturnCode(int code);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
