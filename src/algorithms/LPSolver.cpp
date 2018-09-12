#include "LPSolver.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <base-logging/Logging.hpp>
#include "../MapInitializer.hpp"
#include "../utils/MD5.hpp"

#include "lp/CBCSolver.hpp"
#include "lp/GLPKSolver.hpp"
#include "lp/ScipSolver.hpp"

#ifdef EMBED_GLPK
#include "lp/embedded/GLPKSolver.hpp"
#endif
#ifdef EMBED_SCIP
#include "lp/embedded/ScipSolver.hpp"
#include "lp/embedded/SoplexSolver.hpp"
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
    (LPSolver::CBC_SOLVER, "CBC_SOLVER")
    (LPSolver::GLPK_SOLVER, "GLPK_SOLVER")
    (LPSolver::SCIP_SOLVER, "SCIP_SOLVER")
    (LPSolver::SOPLEX_SOLVER, "SOPLEX_SOLVER")
    (LPSolver::GLPK_SOLVER_EMBEDDED, "GLPK_SOLVER_EMBEDDED")
    (LPSolver::SCIP_SOLVER_EMBEDDED, "SCIP_SOLVER_EMBEDDED")
    (LPSolver::SOPLEX_SOLVER_EMBEDDED, "SOPLEX_SOLVER_EMBEDDED")
    (LPSolver::LP_SOLVER_TYPE_END, "LP_SOLVER_TYPE_END")
    ;

std::map<LPSolver::ProblemFormat, std::string> LPSolver::ProblemFormatTxt =
    InitMap<LPSolver::ProblemFormat, std::string>
    (LPSolver::UNKNOWN_PROBLEM_FORMAT, "unknown problem format")
    (LPSolver::CPLEX, "CPLEX")
    (LPSolver::GLPK, "GLPK")
    (LPSolver::MPS, "MPS");

std::map<LPSolver::ProblemFormat, std::vector<std::string> > LPSolver::ProblemFormatExtensions =
    InitMap<LPSolver::ProblemFormat, std::vector<std::string> >
    (LPSolver::UNKNOWN_PROBLEM_FORMAT, {})
    (LPSolver::CPLEX, {".lp"})
    (LPSolver::GLPK, {".glpk"})
    (LPSolver::MPS, {".mps"} );

LPSolver::LPSolver()
    : mSolutionCacheSize(100)
{}

LPSolver::~LPSolver()
{}

LPSolver::Ptr LPSolver::getInstance(LPSolver::Type solverType)
{
    switch(solverType)
    {
        case CBC_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::CBCSolver>() );
        case GLPK_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::GLPKSolver>() );
        case SCIP_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::ScipSolver>() );
        //case SOPLEX_SOLVER:
        //    return dynamic_pointer_cast<LPSolver>( make_shared<SoplexSolver>() );
        case GLPK_SOLVER_EMBEDDED:
#ifdef EMBED_GLPK
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::embedded::GLPKSolver>() );
#else
            throw
                std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance:"
                        " requested GLPK embedded solver, but support has not"
                        "compiled into the library. Recompile with"
                        "EMBED_GLPK=1");
#endif
        case SCIP_SOLVER_EMBEDDED:
#ifdef EMBED_SCIP
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::embedded::ScipSolver>() );
#else
            throw
                std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance:"
                        " requested SCIP embedded solver, but support has not"
                        "compiled into the library. Recompile with"
                        "EMBED_SCIP=1");
#endif
        case SOPLEX_SOLVER_EMBEDDED:
#ifdef EMBED_SCIP
            return dynamic_pointer_cast<LPSolver>( make_shared<lp::embedded:SoplexSolver>() );
#else
            throw
                std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance:"
                        " requested SOPLEX embedded solver, but support has not"
                        "compiled into the library. Recompile with"
                        "EMBED_SCIP=1");
#endif
        default:
            throw std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance: unknown solver type provided");

    }
}

LPSolver::ProblemFormat getProblemFormatFromFileExtension(const std::string& extension)
{
    for(const std::pair<LPSolver::ProblemFormat, std::vector<std::string> >& p : LPSolver::ProblemFormatExtensions)
    {
        if(p.second.end() != std::find(p.second.begin(), p.second.end(), extension))
        {
            return p.first;
        }
    }
    throw std::invalid_argument("graph_analysis::algorithms::LPSolver::getProblemFormatFromFileExtension:"
            " could not find format for extension: '" + extension + "'");
}

double LPSolver::getVariableValueByColumnIdx(uint32_t idx) const
{
    throw std::runtime_error("graph_analysis::algorithms::LPSolver::getVariableValueByColumnIdx"
            "not implemented");
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

std::string LPSolver::getTempFilename(const std::string& suffix)
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();
    return temp.native() + suffix;
}

std::string LPSolver::saveProblemToTempfile(LPSolver::ProblemFormat format) const
{
    mProblemFile = getTempFilename("-problem.lp");
    mProblemFileFormat = format;

    LOG_INFO_S << "Saving problem to: " << mProblemFile;
    saveProblem(mProblemFile, mProblemFileFormat);
    return mProblemFile;
}

std::string LPSolver::saveSolutionToTempfile(LPSolver::SolutionType format) const
{
    mSolutionFile = getTempFilename("-solution.lp");
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
    if(msKnownSolutions.size() > mSolutionCacheSize)
    {
        resetSolutionCache();
    }

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
