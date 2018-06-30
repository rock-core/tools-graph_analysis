#include "LPSolver.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <base-logging/Logging.hpp>
#include "../MapInitializer.hpp"
#include "../utils/MD5.hpp"

#ifdef WITH_GLPK
#warning Using GLPK solver
#include "GLPKSolver.hpp"
#endif

#ifdef WITH_SCIP
#warning Using SCIP solver
#include "ScipSolver.hpp"
#include "SoplexSolver.hpp"
#endif

namespace graph_analysis {
namespace algorithms {

LPSolver::KnownSolutions LPSolver::msKnownSolutions;

std::map<LPSolver::Status, std::string> LPSolver::StatusTxt =
    InitMap<LPSolver::Status, std::string>
    (LPSolver::STATUS_UNKNOWN, "STATUS_UNKNOWN")
    (LPSolver::STATUS_OPTIMAL, "STATUS_OPTIMAL")
    (LPSolver::STATUS_INFEASIBLE,"STATUS_INFEASIBLE")
    (LPSolver::STATUS_UNBOUNDED, "STATUS_UNBOUNDED")
    (LPSolver::INVALID_PROBLEM_DEFINITION, "INVALID_PROBLEM_DEFINITION")
    (LPSolver::NO_SOLUTION_FOUND, "NO_SOLUTION_FOUND")
    (LPSolver::SOLUTION_FOUND, "SOLUTION_FOUND")
    ;

std::map<LPSolver::Type, std::string> LPSolver::TypeTxt =
    InitMap<LPSolver::Type, std::string>
    (LPSolver::UNKNOWN_LP_SOLVER, "UNKNOWN_LP_SOLVER")
    (LPSolver::GLPK_SOLVER, "GLPK_SOLVER")
    (LPSolver::SCIP_SOLVER, "SCIP_SOLVER")
    (LPSolver::SOPLEX_SOLVER, "SOPLEX_SOLVER")
    (LPSolver::LP_SOLVER_TYPE_END, "LP_SOLVER_TYPE_END")
    ;

std::map<LPSolver::ProblemFormat, std::string> LPSolver::ProblemFormatTxt =
    InitMap<LPSolver::ProblemFormat, std::string>
    (LPSolver::UNKNOWN_PROBLEM_FORMAT, "unknown problem format")
    (LPSolver::CPLEX, "CPLEX")
    (LPSolver::GLPK, "GLPK")
    (LPSolver::MPS, "MPS");


LPSolver::~LPSolver()
{}

LPSolver::Ptr LPSolver::getInstance(LPSolver::Type solverType)
{
    switch(solverType)
    {
        case GLPK_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<GLPKSolver>() );
#ifdef WITH_SCIP
        case SCIP_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<ScipSolver>() );
        case SOPLEX_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<SoplexSolver>() );
#endif
        default:
            throw std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance: unknown solver type provided");

    }
}

std::string LPSolver::getVariableNameByColumnIdx(uint32_t idx) const
{
    std::stringstream ss;
    ss << "x" << idx;
    return ss.str();
}

uint32_t LPSolver::getColumnIdxByVariableName(const std::string& varName) const
{
    uint32_t index;
    std::stringstream ss;
    ss << varName.substr(1);
    ss >> index;
    return index;
}

std::string LPSolver::saveProblemToTempfile(LPSolver::ProblemFormat format) const
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();
    mProblemFile = temp.native() + "-problem.lp";
    mProblemFileFormat = format;

    LOG_INFO_S << "Saving problem to: " << mProblemFile;
    saveProblem(mProblemFile, mProblemFileFormat);
    return mProblemFile;
}

std::string LPSolver::saveSolutionToTempfile(LPSolver::SolutionType format) const
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();
    mSolutionFile = temp.native() + ".lp";
    mSolutionFileFormat = format;

    LOG_INFO_S << "Saving solution to: " << mSolutionFile;
    saveSolution(mSolutionFile, mSolutionFileFormat);
    return mSolutionFile;
}

bool LPSolver::loadProblem(const std::string& filename, ProblemFormat format)
{
    std::string md5 = utils::MD5Digest::md5Sum(filename);
    bool solutionIsKnown = (msKnownSolutions.count(md5) != 0);
    try {
        doLoadProblem(filename, format);
    } catch(const std::exception& e)
    {
        throw std::runtime_error("graph_analysis::algorithms::LPSolver::loadProblem: "
                "failed to load problem file '" + filename + "' with format '" + ProblemFormatTxt[format] + " : " + e.what());
    }
    return solutionIsKnown;

}

void LPSolver::registerSolution(const std::string& problemFilename,
        const std::string& solutionFilename, Status status, SolutionType type)
{
    std::string md5 = utils::MD5Digest::md5Sum(problemFilename);
    msKnownSolutions[md5] = KnownSolution(solutionFilename, status, type);
}

const LPSolver::KnownSolutions::mapped_type& LPSolver::getRegisteredSolution(const std::string& problemFilename) const
{
    std::string md5 = utils::MD5Digest::md5Sum(problemFilename);
    KnownSolutions::const_iterator cit = msKnownSolutions.find(md5);
    if(cit != msKnownSolutions.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("graph_analysis::algorithms::LPSolver::getRegisteredSolution: could not find a registered solution for problem '" + problemFilename + "'");
}

LPSolver::Status LPSolver::loadKnownSolution(const std::string& problemFilename)
{
    KnownSolution p = getRegisteredSolution(problemFilename);
    loadSolution(p.filename, p.type);
    return p.status;
}


} // end namespace algorithms
} // end graph_analysis
