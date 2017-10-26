#ifndef GRAPH_ANALYSIS_ALGORITHMS_SCIP_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_SCIP_SOLVER_HPP

#include <scip/scip.h>
#include "LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {

class ScipSolver : public LPSolver
{
public:
    ScipSolver();
    virtual ~ScipSolver();

    typedef shared_ptr<ScipSolver> Ptr;

    void debugSolution(bool enable);

    LPSolver::Type getSolverType() const override { return SCIP_SOLVER; }

    void doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) override;

    void saveProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) const override;

    void loadSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION);

    void saveSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) const override;

    double getObjectiveValue() const override;

    double getVariableValue(const std::string& varName) const;

    double getVariableValueByColumnIdx(uint32_t idx) const override;

    Status run() override;

    Status getStatus() const;

    Status run(const std::string& problem, LPSolver::ProblemFormat problemFormat = CPLEX, bool useCaching = false) override;

private:
    SCIP* mpScip;
    bool mDebugSolution;

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_SCIP_SOLVER_HPP
