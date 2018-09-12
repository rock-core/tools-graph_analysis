#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_GLPK_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_GLPK_SOLVER_HPP

#include <string>
#include <stdexcept>
#include "CommandlineSolver.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

/**
 * General class to call the
 */
class GLPKSolver : public CommandlineSolver
{
public:
    GLPKSolver();
    virtual ~GLPKSolver();

    LPSolver::Type getSolverType() const override { return GLPK_SOLVER; }

    virtual std::string createSolverCommand() const;

    virtual Solution readBasicSolution(const std::string& filename);
};

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_GLPK_SOLVER_HPP
