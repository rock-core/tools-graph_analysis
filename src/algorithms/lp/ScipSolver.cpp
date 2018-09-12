#include "ScipSolver.hpp"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {

std::string ScipSolver::createSolverCommand() const
{
    std::string cmd;
    cmd += "scip";
    cmd += " -f " + mProblemFile;
    cmd += " -l " + mSolutionFile;
    cmd += " > /dev/null";
    return cmd;
}

Solution ScipSolver::readBasicSolution(const std::string& filename)
{
    Solution solution;
    std::ifstream solutionFile;
    solutionFile.open(filename.c_str(), std::ios::in);

    std::string line;
    bool parseColumns = false;
    while(std::getline(solutionFile, line))
    {

        if(boost::starts_with(line, "SCIP Status"))
        {
            if (line.find("optimal") != std::string::npos)
            {
                solution.setStatus(LPSolver::STATUS_OPTIMAL);
            } else if(line.find("infeasible") != std::string::npos)
            {
                solution.setStatus(LPSolver::STATUS_INFEASIBLE);
                return solution;
            } else {
                solution.setStatus(LPSolver::STATUS_UNKNOWN);
            }
            continue;
        }

        std::vector<std::string> splitLine;
        boost::trim_if(line, boost::is_any_of("\t "));
        // https://www.boost.org/doc/libs/1_49_0/doc/html/boost/algorithm/split_id820181.html
        // token_compress_on: adjacent separators are merged together. Otherwise, every two separators delimit a token.
        boost::split(splitLine, line, boost::is_any_of("\t "), boost::token_compress_on);
        if(splitLine.empty())
        {
            continue;
        }

        if(boost::starts_with(line, "Primal Bound"))
        {
            // -- Parsing the header
            // Optimal - objective value 6.00000000
            double objectiveValue = 0;
            try {
                objectiveValue = boost::lexical_cast<double>(splitLine[3]);
            } catch(const std::bad_cast& e)
            {
                LOG_WARN_S << "Failed to extract objective value from '"
                    + splitLine[3] + "'";
                throw;
            }
            solution.setObjectiveValue(objectiveValue);
            continue;
        }
        if(boost::starts_with(line, "objective value"))
        {
            parseColumns = true;
            continue;
        }

        if(parseColumns)
        {
            std::string columnName = splitLine[0];
            if(boost::starts_with(columnName, "x"))
            {
                double value;
                try {
                    value = boost::lexical_cast<size_t>(splitLine[1]);
                } catch(const std::bad_cast& e)
                {
                    LOG_WARN_S << "Failed to extract column value from '"
                        + splitLine[1] + "' -- solution file: " +  filename;
                    throw;
                }
                solution.setColumnValue(columnName, value);
            }
        }

        if(boost::starts_with(line, "Statistics"))
        {
            parseColumns = false;
        }
    }

    solutionFile.close();
    return solution;

}

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
