#ifndef GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP

#include <glpk.h>
#include <string>
#include <stdexcept>
#include "../../LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {
namespace embedded {

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

    LPSolver::Type getSolverType() const override { return GLPK_SOLVER; }

    std::string getProblemName() const { return std::string( glp_get_prob_name(mpProblem) ) ; }

    void doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) override;

    void saveProblem(const std::string& filename, LPSolver::ProblemFormat format = CPLEX) const override;

    void loadSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) override;

    void saveSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) const override;

    double getObjectiveValue() const override { return glp_get_obj_val(mpProblem); };

    double getVariableValue(const std::string& varName) const;

    double getVariableValueByColumnIdx(uint32_t idx) const override;

    Status run() override { throw std::runtime_error("graph_analysis::algorithms::GLPKSolver::run not implemented"); }

    Status run(const std::string& problem, LPSolver::ProblemFormat problemFormat = CPLEX, bool useCaching = false) override;

    /**
     * Hook function to redirect the output of glpk to the logger
     */
    static int printHook(void* info, const char* s);

protected:
    glp_prob* mpProblem;

    Status translateSimplexReturnCode(int code);
    Status translateIntoptReturnCode(int code);
};

} // end namespace embedded
} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_GLPK_SOLVER_HPP
