#ifndef GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP

#include <glpk.h>
#include <string>
#include <stdexcept>

namespace graph_analysis {
namespace algorithms {

enum LPProblemFormat { UNKNOWN_PROBLEM_FORMAT = 0, CPLEX, GLPK, MPS };
enum LPSolutionFormat { UNKNOWN_SOLUTION_FORMAT = 0 };

class GLPKSolver
{
public:
    enum Status { INVALID_PROBLEM_DEFINITION, NO_SOLUTION_FOUND, SOLUTION_FOUND };

    GLPKSolver(const std::string& problemName);
    virtual ~GLPKSolver();

    std::string getProblemName() const { return std::string( glp_get_prob_name(mpProblem) ) ; }

    void saveProblem(const std::string& filename, LPProblemFormat format = CPLEX);

    void saveSolution(const std::string& filename, LPSolutionFormat format = UNKNOWN_SOLUTION_FORMAT);

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
