#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_COMMAND_LINE_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_COMMAND_LINE_SOLVER_HPP

#include <string>
#include <stdexcept>
#include "../LPSolver.hpp"
#include "Solution.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

/**
 * General class to call the
 */
class CommandlineSolver : public LPSolver
{
public:
    CommandlineSolver();
    virtual ~CommandlineSolver();

    std::string getProblemName() const { return "unknown-problem-name"; }

    void doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) override;

    void saveProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) const override;

    void loadSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) override;

    void saveSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) const override;

    double getObjectiveValue() const override { return mSolution.getObjectiveValue(); }

    double getVariableValueByColumnIdx(uint32_t idx) const override;

    double getVariableValue(const std::string& varName) const { return mSolution.getColumnValue(varName); }

    Status run() override;

    Status run(const std::string& problem, LPSolver::ProblemFormat problemFormat = CPLEX, bool useCaching = false) override;

    virtual std::string createSolverCommand() const = 0;

    virtual Solution readBasicSolution(const std::string& filename) = 0;

protected:
    Solution mSolution;

};

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_COMMAND_LINE_SOLVER_HPP
