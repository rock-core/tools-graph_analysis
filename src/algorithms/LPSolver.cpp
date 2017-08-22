#include "LPSolver.hpp"
#include <boost/filesystem.hpp>

#ifdef WITH_GLPK
#warning Using GLPK solver
#include "GLPKSolver.hpp"
#endif

#ifdef WITH_SCIP
#warning Using SCIP solver
#include "ScipSolver.hpp"
#endif

namespace graph_analysis {
namespace algorithms {

LPSolver::~LPSolver()
{}

LPSolver::Ptr LPSolver::getInstance(LPSolverType solverType)
{
    switch(solverType)
    {
        case GLPK_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<GLPKSolver>() );
#ifdef WITH_SCIP
        case SCIP_SOLVER:
            return dynamic_pointer_cast<LPSolver>( make_shared<ScipSolver>() );
#endif
        default:
            throw std::invalid_argument("graph_analysis::algorithms::LPSolver::getInstance: unknown solver type provided");

    }
}

std::string LPSolver::saveSolutionToTempfile(LPSolutionType format) const
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();
    mSolutionFile = temp.native() + ".lp";
    mSolutionFileFormat = format;

    saveSolution(mSolutionFile, mSolutionFileFormat);
    return mSolutionFile;
}

} // end namespace algorithms
} // end graph_analysis
