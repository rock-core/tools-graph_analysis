#ifndef GRAPH_ANALYSIS_ALGORITHMS_SOPLEX_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_SOPLEX_SOLVER_HPP

#include <soplex.h>
#include "LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {

class SoplexSolver : public LPSolver
{
public:
    SoplexSolver();
    virtual ~SoplexSolver();

    LPSolver::Type getSolverType() const override { return SOPLEX_SOLVER; }

    void saveProblem(const std::string& filename, ProblemFormat format = CPLEX) const override;

    void loadSolution(const std::string& filename, SolutionType format = BASIC_SOLUTION) override;

    /**
     * Save the solution to file
     * \param filename filename of the solution file
     * \param format format of the the solution
     */
    void saveSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) const override;

    /**
     * Retrieve the current objective value of the solution
     */
    double getObjectiveValue() const override;

    /**
     * Get the variable value by the column index
     * \param idx column index
     * \return the variable value
     */
    double getVariableValueByColumnIdx(uint32_t idx) const override;

    Status run() override { throw std::runtime_error("graph_analysis::algorithms::SoplexSolver::run not implemented"); }

    Status run(const std::string& problem, LPSolver::ProblemFormat problemFormat = CPLEX, bool useCaching = false) override;

    std::vector<size_t> infeasibleConstraints() const override { return mInfeasibleConstraints; }

protected:
    soplex::SoPlex mSoplex;
    soplex::NameSet mRowNames;
    soplex::NameSet mColumnNames;
    std::vector<size_t> mInfeasibleConstraints;
    virtual void doLoadProblem(const std::string& filename, ProblemFormat format = CPLEX);

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_SOPLEX_SOLVER_HPP
