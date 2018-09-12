#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_SCIP_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_SCIP_SOLVER_HPP

#include <string>
#include <stdexcept>
#include "CommandlineSolver.hpp"
#include "Solution.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

/**
 * General class to call the Scip solver
 */
class ScipSolver : public CommandlineSolver
{
public:
    LPSolver::Type getSolverType() const override { return SCIP_SOLVER; }

    std::string createSolverCommand() const override;
    Solution readBasicSolution(const std::string& filename) override;
};

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_SCIP_SOLVER_HPP
