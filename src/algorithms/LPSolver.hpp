#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP

#include <string>
#include "../SharedPtr.hpp"

namespace graph_analysis {
namespace algorithms {

enum LPSolverType { UNKNOWN_LP_SOLVER, GLPK_SOLVER, SCIP_SOLVER };

enum LPProblemFormat { UNKNOWN_PROBLEM_FORMAT = 0, CPLEX, GLPK, MPS };

/// Solution types: basic, IPT = interior point, MIP = mixed integer
enum LPSolutionType { UNKNOWN_SOLUTION_TYPE = 0, BASIC_SOLUTION, IPT_SOLUTION, MIP_SOLUTION };


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

    virtual ~LPSolver();

    typedef shared_ptr<LPSolver> Ptr;

    static LPSolver::Ptr getInstance(LPSolverType solverType);

    virtual LPSolverType getSolverType() const = 0;

    /**
     * Load a problem based
     * \param filename filename of the problem file
     * \param format format of the problem
     */
    virtual void loadProblem(const std::string& filename, LPProblemFormat format = CPLEX) = 0;

    /**
     * Save the problem to file
     * \param filename filename of the problem file
     * \param format format of the the problem
     */
    virtual void saveProblem(const std::string& filename, LPProblemFormat format = CPLEX) const = 0;

    /**
     * Load a solution based
     * \param filename filename of the solution file
     * \param format format of the solution
     */
    virtual void loadSolution(const std::string& filename, LPSolutionType format = BASIC_SOLUTION) = 0;


    /**
     * Save the solution in a temporary file
     * \return the temporary file where the solution has been saved to
     */
    std::string saveSolutionToTempfile(LPSolutionType format = BASIC_SOLUTION) const;

    /**
     * Save the solution to file
     * \param filename filename of the solution file
     * \param format format of the the solution
     */
    virtual void saveSolution(const std::string& filename, LPSolutionType format = BASIC_SOLUTION) const = 0;

    /**
     * Retrieve the current objective value of the solution
     */
    virtual double getObjectiveValue() const = 0;

    /**
     * Run the solver (after loadProblem) has been called
     */
    virtual Status run() = 0;

    /**
     * Run the solver with a given problem file
     * \param problemFilename filename of the problem to solve
     * \param problemFormat format of the problem to solver
     */
    virtual Status run(const std::string& problemFilename, LPProblemFormat problemFormat = CPLEX) = 0;

    const std::string& getProblemFile() const { return mProblemFile; }
    LPProblemFormat getProblemFileFormat() const { return mProblemFileFormat; }

    const std::string& getSolutionFile() const { return mSolutionFile; }
    LPSolutionType getSolutionFileFormat() const { return mSolutionFileFormat; }

protected:
    /// Provide the current problem file and format
    /// after loadProblem has been called
    mutable std::string mProblemFile;
    mutable LPProblemFormat mProblemFileFormat;
    //
    /// Provide the current solution file and format
    /// after loadSolution has been called
    mutable std::string mSolutionFile;
    mutable LPSolutionType mSolutionFileFormat;
};


} // end namespace algorithms
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
