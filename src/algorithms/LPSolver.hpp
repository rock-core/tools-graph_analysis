#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP

#include <string>
#include <map>
#include "../SharedPtr.hpp"

namespace graph_analysis {
namespace algorithms {

/**
 * General interface to call linear problem solvers
 * currently GLPK and SCIP
 */
class LPSolver
{
public:
    enum Status { STATUS_UNKNOWN,
        STATUS_OPTIMAL,
        STATUS_INFEASIBLE,
        STATUS_UNBOUNDED,
        INVALID_PROBLEM_DEFINITION,
        NO_SOLUTION_FOUND,
        SOLUTION_FOUND };

    enum Type { UNKNOWN_LP_SOLVER, GLPK_SOLVER, SCIP_SOLVER, LP_SOLVER_TYPE_END };

    static std::map<Type, std::string> TypeTxt;

    enum ProblemFormat { UNKNOWN_PROBLEM_FORMAT = 0, CPLEX, GLPK, MPS, LP_PROBLEM_FORMAT_END};

    /// Solution types: basic, IPT = interior point, MIP = mixed integer
    enum SolutionType { UNKNOWN_SOLUTION_TYPE = 0, BASIC_SOLUTION, IPT_SOLUTION, MIP_SOLUTION, LP_SOLUTION_TYPE_END };

    virtual ~LPSolver();

    typedef shared_ptr<LPSolver> Ptr;

    static LPSolver::Ptr getInstance(LPSolver::Type solverType);

    virtual LPSolver::Type getSolverType() const = 0;

    /**
     * Load a problem based
     * \param filename filename of the problem file
     * \param format format of the problem
     */
    virtual void loadProblem(const std::string& filename, ProblemFormat format = CPLEX) = 0;

    /**
     * Save the problem to file
     * \param filename filename of the problem file
     * \param format format of the the problem
     */
    virtual void saveProblem(const std::string& filename, ProblemFormat format = CPLEX) const = 0;

    /**
     * Load a solution based
     * \param filename filename of the solution file
     * \param format format of the solution
     */
    virtual void loadSolution(const std::string& filename, SolutionType format = BASIC_SOLUTION) = 0;

    /**
     * Save the problem in a temporary file
     * \return the temporary file where the problem has been saved to
     */
    std::string saveProblemToTempfile(LPSolver::ProblemFormat format = CPLEX) const;

    /**
     * Save the solution in a temporary file
     * \return the temporary file where the solution has been saved to
     */
    std::string saveSolutionToTempfile(LPSolver::SolutionType format = BASIC_SOLUTION) const;

    /**
     * Save the solution to file
     * \param filename filename of the solution file
     * \param format format of the the solution
     */
    virtual void saveSolution(const std::string& filename, LPSolver::SolutionType format = BASIC_SOLUTION) const = 0;

    /**
     * Retrieve the current objective value of the solution
     */
    virtual double getObjectiveValue() const = 0;

    /**
     * Get the variable value by the column index
     * \param idx column index
     * \return the variable value
     */
    virtual double getVariableValueByColumnIdx(uint32_t idx) const = 0;

    /**
     * Get the canonized variable name for the given column index
     * \param idx column index
     * \return the variable name
     */
    std::string getVariableNameByColumnIdx(uint32_t idx) const;

    /**
     * Get the column index for the canonize variable name
     * \param varName canonized variable name
     * \return the column index
     */
    uint32_t getColumnIdxByVariableName(const std::string& varName) const;

    /**
     * Run the solver (after loadProblem) has been called
     */
    virtual Status run() = 0;

    /**
     * Run the solver with a given problem file
     * \param problemFilename filename of the problem to solve
     * \param problemFormat format of the problem to solver
     */
    virtual Status run(const std::string& problemFilename, LPSolver::ProblemFormat problemFormat = CPLEX) = 0;

    const std::string& getProblemFile() const { return mProblemFile; }
    LPSolver::ProblemFormat getProblemFileFormat() const { return mProblemFileFormat; }

    const std::string& getSolutionFile() const { return mSolutionFile; }
    LPSolver::SolutionType getSolutionFileFormat() const { return mSolutionFileFormat; }

protected:
    /// Provide the current problem file and format
    /// after loadProblem has been called
    mutable std::string mProblemFile;
    mutable LPSolver::ProblemFormat mProblemFileFormat;
    //
    /// Provide the current solution file and format
    /// after loadSolution has been called
    mutable std::string mSolutionFile;
    mutable LPSolver::SolutionType mSolutionFileFormat;
};


} // end namespace algorithms
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
