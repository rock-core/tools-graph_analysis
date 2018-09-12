#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_CBC_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_CBC_SOLVER_HPP

#include <string>
#include <stdexcept>
#include "CommandlineSolver.hpp"
#include "Solution.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

/**
 * General class to call the
 */
class CBCSolver : public CommandlineSolver
{
public:
    LPSolver::Type getSolverType() const override { return CBC_SOLVER; }

    std::string createSolverCommand() const override;
    Solution readBasicSolution(const std::string& filename) override;
};

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_CBC_SOLVER_HPP
