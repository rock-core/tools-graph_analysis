#include "CommandlineSolver.hpp"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <base-logging/Logging.hpp>
#include "../../utils/Filesystem.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

CommandlineSolver::CommandlineSolver()
{}

CommandlineSolver::~CommandlineSolver()
{
}

void CommandlineSolver::doLoadProblem(const std::string& filename, LPSolver::ProblemFormat format)
{
    mProblemFile = filename;
    mProblemFileFormat = format;
}

void CommandlineSolver::saveProblem(const std::string& filename, LPSolver::ProblemFormat format) const
{
    if(format != mProblemFileFormat)
    {
        throw std::invalid_argument("graph_analysis::algorithms::saveProblem:"
                " saving problem in format '" + LPSolver::ProblemFormatTxt[format] + "' requested"
                ", but can only be saved as '" + LPSolver::ProblemFormatTxt[mProblemFileFormat] + "'");
    }

    utils::Filesystem::copy(mProblemFile, filename);

}

void CommandlineSolver::saveSolution(const std::string& filename, LPSolver::SolutionType format) const
{
    utils::Filesystem::copy(mSolutionFile, filename);
}

void CommandlineSolver::loadSolution(const std::string& filename, LPSolver::SolutionType format)
{
    switch(format)
    {
        case BASIC_SOLUTION:
            mSolution = readBasicSolution(filename);
            break;
        //case IPT_SOLUTION:
        //    break;
        //case MIP_SOLUTION:
        //    break;
        default:
            throw std::invalid_argument("CommandlineSolver: " + getProblemName() + " failed to read solution to '" + filename + "' since selected solution type is unknown/not supported");
    }
}

LPSolver::Status CommandlineSolver::run()
{
    mSolutionFile = LPSolver::getTempFilename("-solution.lp");
    mSolutionFileFormat = LPSolver::BASIC_SOLUTION;

    std::string cmd = createSolverCommand();
    if(-1 == system(cmd.c_str()))
    {

        throw std::runtime_error("graph_analysis::algorithms::lp::CommandlineSolver:"
                " failed to run solver with cmd '" + cmd + "'");
    } else {
        mSolution = readBasicSolution(mSolutionFile);
    }
    return mSolution.getStatus();
}

LPSolver::Status CommandlineSolver::run(const std::string& problem,
        LPSolver::ProblemFormat problemFormat, bool useCaching)
{
    CommandlineSolver::Status status = CommandlineSolver::STATUS_UNKNOWN;
    bool knownProblem = loadProblem(problem, problemFormat);
    try {
        if(knownProblem && useCaching)
        {
            status = loadKnownSolution(problem);
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "Error loading problem file: " << e.what();
    }

    if(status == CommandlineSolver::STATUS_UNKNOWN)
    {
        status = run();
        if(useCaching)
        {
            registerSolution(mProblemFile, mSolutionFile, status, BASIC_SOLUTION);
        }
    }
    return status;
}

double CommandlineSolver::getVariableValueByColumnIdx(uint32_t idx) const
{
    std::string columnName = getVariableNameByColumnIdx(idx);
    return getVariableValue(columnName);
}

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
