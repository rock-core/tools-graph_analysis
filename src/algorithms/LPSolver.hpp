#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP

#include <string>
#include <map>
#include <vector>
#include "../SharedPtr.hpp"

namespace graph_analysis {
namespace algorithms {

/**
 * General interface to call linear problem solvers
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

    static std::map<Status, std::string> StatusTxt;

    enum Type { UNKNOWN_LP_SOLVER,
        CBC_SOLVER,
        CLP_SOLVER,
        GLPK_SOLVER,
        SCIP_SOLVER,
        SOPLEX_SOLVER,
        GLPK_SOLVER_EMBEDDED,
        SCIP_SOLVER_EMBEDDED,
        SOPLEX_SOLVER_EMBEDDED,
        LP_SOLVER_TYPE_END };

    static std::map<Type, std::string> TypeTxt;

    enum ProblemFormat { UNKNOWN_PROBLEM_FORMAT = 0, CPLEX, GLPK, MPS, LP_PROBLEM_FORMAT_END};
    static std::map<ProblemFormat, std::string> ProblemFormatTxt;
    static std::map<LPSolver::ProblemFormat, std::vector<std::string> > ProblemFormatExtensions;

    /// Solution types: basic, IPT = interior point, MIP = mixed integer
    enum SolutionType { UNKNOWN_SOLUTION_TYPE = 0, BASIC_SOLUTION, IPT_SOLUTION, MIP_SOLUTION, LP_SOLUTION_TYPE_END };

    struct KnownSolution
    {
        std::string filename;
        Status status;
        SolutionType type;

        KnownSolution()
        {}

        KnownSolution(const std::string& filename, Status status, SolutionType type)
            : filename(filename)
            , status(status)
            , type(type)
        {}
    };

    LPSolver();

    virtual ~LPSolver();

    typedef shared_ptr<LPSolver> Ptr;

    static LPSolver::Ptr getInstance(LPSolver::Type solverType);

    /**
     * Get the problem format from an extension
     */
    static ProblemFormat getProblemFormatFromFileExtension(const std::string& extension);

    virtual LPSolver::Type getSolverType() const = 0;

    /**
     * Load a problem based
     * \param filename filename of the problem file
     * \param format format of the problem
     * \return if the problem is already known
     */
    bool loadProblem(const std::string& filename, ProblemFormat format = CPLEX);

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
     * Get a temporary filename
     * \return temporary filename
     */
    static std::string getTempFilename(const std::string& suffix = ".lp");

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
    virtual double getVariableValueByColumnIdx(uint32_t idx) const;

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
     * \param useCaching To use caching of problem and respective solutions set
     * to True, otherwise False
     */
    virtual Status run(const std::string& problemFilename, LPSolver::ProblemFormat problemFormat = CPLEX, bool useCaching = false) = 0;

    const std::string& getProblemFile() const { return mProblemFile; }
    LPSolver::ProblemFormat getProblemFileFormat() const { return mProblemFileFormat; }

    const std::string& getSolutionFile() const { return mSolutionFile; }
    LPSolver::SolutionType getSolutionFileFormat() const { return mSolutionFileFormat; }

    /**
     * Allow to return the set of infeasible constraint by a list of indices in
     * the final solution
     */
    virtual std::vector<size_t> infeasibleConstraints() const { throw std::runtime_error("graph_analysis::algorithms::LPSolver::infeasibleCconstraints: not implemented"); }

    /**
      * Set the maximum size of the solution cache
      * The cache will be cleared -- if the size exceeds this cache size
      */
    void setSolutionCacheSize(size_t size) { mSolutionCacheSize = size; }
    size_t getSolutionCacheSize() const { return mSolutionCacheSize; }
    void resetSolutionCache() { msKnownSolutions.clear(); }

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

    typedef std::map<std::string, KnownSolution> KnownSolutions;
    static KnownSolutions msKnownSolutions;
    size_t mSolutionCacheSize;

    virtual void doLoadProblem(const std::string& filename, ProblemFormat format = CPLEX) = 0;

    Status loadKnownSolution(const std::string& problemFilename);

    void registerSolution(const std::string& problemFilename,
        const std::string& solutionFilename,
        Status status,
        SolutionType type
        );

    const LPSolver::KnownSolutions::mapped_type& getRegisteredSolution(const std::string& problemFilename) const;
};


} // end namespace algorithms
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_SOLVER_HPP
