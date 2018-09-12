#include "GLPKSolver.hpp"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <base-logging/Logging.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {

GLPKSolver::GLPKSolver()
    : CommandlineSolver()
{}

GLPKSolver::~GLPKSolver()
{
}

std::string GLPKSolver::createSolverCommand() const
{
    std::string cmd;
    cmd += "glpsol";
    cmd += " --lp " + mProblemFile;
    cmd += " -o " + mSolutionFile;
    cmd += " > /dev/null";
    return cmd;
}

Solution GLPKSolver::readBasicSolution(const std::string& filename)
{
    Solution solution;
    std::ifstream solutionFile;
    solutionFile.open(filename.c_str(), std::ios::in);

    std::string line;
    bool parseHeader = true;
    bool parseRows = false;
    bool parseColumns = false;
    while(std::getline(solutionFile, line))
    {
        std::vector<std::string> splitLine;
        boost::trim_if(line, boost::is_any_of("\t "));
        // https://www.boost.org/doc/libs/1_49_0/doc/html/boost/algorithm/split_id820181.html
        // token_compress_on: adjacent separators are merged together. Otherwise, every two separators delimit a token.
        boost::split(splitLine, line, boost::is_any_of("\t "), boost::token_compress_on);
        if(splitLine.empty())
        {
            continue;
        }

        if(parseHeader)
        {
            std::string keyword = splitLine[0];
            // -- Parsing the header
            //
            //     Problem:
            //     Rows:       47
            //     Columns:    46 (23 integer, 1 binary)
            //     Non-zeros:  97
            //     Status:     INTEGER OPTIMAL
            //     Objective:  obj = 6 (MINimum)
            if( keyword == "Rows:")
            {
                size_t n = boost::lexical_cast<size_t>(splitLine[1]);
                solution.setNumberOfRows(n);
            } else if(keyword == "Columns:")
            {
                size_t n = boost::lexical_cast<size_t>(splitLine[1]);
                solution.setNumberOfColumns( boost::lexical_cast<size_t>(n));
            } else if(keyword == "Non-zeros:")
            {
                size_t n = boost::lexical_cast<size_t>(splitLine[1]);
                solution.setNumberOfNonZeros( boost::lexical_cast<size_t>(n));
            } else if(keyword == "Status:")
            {
                std::string status = splitLine[2];
                if(status == "OPTIMAL")
                {
                    solution.setStatus(LPSolver::STATUS_OPTIMAL);
                } else if(status == "INFEASIBLE" || status == "EMPTY")
                {
                    solution.setStatus(LPSolver::STATUS_INFEASIBLE);
                } else {
                    solution.setStatus(LPSolver::STATUS_UNKNOWN);
                }

            } else if(keyword == "Objective:")
            {
                if(splitLine.size() > 3)
                {
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
                }
                // Header completed
                parseHeader = false;
            }
        } else if(splitLine.size() > 1)
        {
            if(splitLine[1] == "Row")
            {
                parseRows = true;
                parseColumns = false;
            } else if(splitLine[1] == "Column")
            {
                parseRows = false;
                parseColumns = true;
            }
        }

        if(parseRows)
        {
        } else if(parseColumns)
        {
            if(splitLine.size() > 4)
            {
                // Identify basic variable -- Activity field
                if(splitLine[2] == "*")
                {
                    // Use the activity field to set the column value
                    solution.setColumnValue(splitLine[1], boost::lexical_cast<size_t>(splitLine[3]));
                }
            }
        }

    }

    solutionFile.close();
    return solution;
}

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
