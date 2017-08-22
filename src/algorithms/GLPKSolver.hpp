#ifndef GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP

#include <glpk.h>
#include <string>
#include <stdexcept>
#include "LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {

/**
 * General class to interface with the Gnu Linear Programming Kit
 * \see http://kam.mff.cuni.cz/~elias/glpk.pdf
 */
class GLPKSolver : public LPSolver
{
public:
    GLPKSolver();
    GLPKSolver(const std::string& problemName);
    virtual ~GLPKSolver();

    LPSolverType getSolverType() const override { return GLPK_SOLVER; }

    std::string getProblemName() const { return std::string( glp_get_prob_name(mpProblem) ) ; }

    void loadProblem(const std::string& filename, LPProblemFormat format = CPLEX) override;

    void saveProblem(const std::string& filename, LPProblemFormat format = CPLEX) const override;

    void loadSolution(const std::string& filename, LPSolutionType format = BASIC_SOLUTION) override;

    void saveSolution(const std::string& filename, LPSolutionType format = BASIC_SOLUTION) const override;

    double getObjectiveValue() const override { return glp_get_obj_val(mpProblem); };

    Status run() override { throw std::runtime_error("graph_analysis::algorithms::GLPKSolver::run not implemented"); }

    Status run(const std::string& problem, LPProblemFormat problemFormat = CPLEX) override;

protected:
    glp_prob* mpProblem;

    Status translateSimplexReturnCode(int code);
    Status translateIntoptReturnCode(int code);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
