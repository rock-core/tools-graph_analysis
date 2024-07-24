#include "CLPSolver.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <base-logging/Logging.hpp>
#include "../../utils/Filesystem.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

std::string CLPSolver::createSolverCommand() const
{
    std::string cmd = utils::Filesystem::validateProgramAvailability("clp");
    cmd += "  " + mProblemFile;
    cmd += " solve";
    cmd += " solu " + mSolutionFile;
    cmd += " > /dev/null";
    return cmd;
}

Solution CLPSolver::readBasicSolution_v2(const std::string& filename)
{
    Solution solution;
    std::ifstream solutionFile;
    solutionFile.open(filename.c_str(), std::ios::in);

    std::string header;
    getline(solutionFile, header);
    boost::to_lower(header);

    std::vector<std::string> splitLine;
    boost::split(splitLine, header, boost::is_any_of("-"), boost::token_compress_on);

    std::string status = splitLine[0];
    boost::trim_if(status, boost::is_any_of(" "));

    if(status == "optimal")
    {
        solution.setStatus(LPSolver::STATUS_OPTIMAL);
    } else if(status == "infeasible")
    {
        solution.setStatus(LPSolver::STATUS_INFEASIBLE);
        return solution;
    } else {
        solution.setStatus(LPSolver::STATUS_UNKNOWN);
    }

    if(boost::contains(header, "objective value"))
    {
        // Optimal - objective value 6.00000000
        std::vector<std::string> objectiveKeyValue;
        std::string objective = splitLine[1];
        boost::trim_if(objective, boost::is_any_of(" "));
        boost::split(objectiveKeyValue, objective, boost::is_any_of(" "), boost::token_compress_on);

        // -- Parsing the header
        double objectiveValue = 0;
        try {
            objectiveValue = boost::lexical_cast<double>(objectiveKeyValue[2]);
        } catch(const std::bad_cast& e)
        {
            LOG_WARN_S << "Failed to extract objective value from '"
                + objective + "'";
            throw;
        }
        solution.setObjectiveValue(objectiveValue);
    }

    std::string line;
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

        {
            if(splitLine.size() > 3 && boost::starts_with(splitLine[1], "x"))
            {
                solution.setColumnValue(splitLine[1], boost::lexical_cast<size_t>(splitLine[2]));
            }
        }
    }

    solutionFile.close();
    return solution;
}

Solution CLPSolver::readBasicSolution_v1(const std::string& filename)
{
    Solution solution;
    std::ifstream solutionFile;
    solutionFile.open(filename.c_str(), std::ios::in);

    std::string line;
    getline(solutionFile, line);
    std::string status = line;
    boost::to_lower(status);
    if(status == "optimal")
    {
        solution.setStatus(LPSolver::STATUS_OPTIMAL);
    } else if(status == "infeasible")
    {
        solution.setStatus(LPSolver::STATUS_INFEASIBLE);
        return solution;
    } else {
        solution.setStatus(LPSolver::STATUS_UNKNOWN);
    }

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

        if(boost::starts_with(line, "Objective value"))
        {
            // -- Parsing the header
            // Optimal
            // Objective value 6.00000000

            double objectiveValue = 0;
            try {
                objectiveValue = boost::lexical_cast<double>(splitLine[2]);
            } catch(const std::bad_cast& e)
            {
                LOG_WARN_S << "Failed to extract objective value from '"
                    + splitLine[2] + "'";
                throw;
            }
            solution.setObjectiveValue(objectiveValue);
        } else
        {
            if(splitLine.size() > 3 && boost::starts_with(splitLine[1], "x"))
            {
                solution.setColumnValue(splitLine[1], boost::lexical_cast<size_t>(splitLine[2]));
            }
        }
    }

    solutionFile.close();
    return solution;
}

Solution CLPSolver::readBasicSolution(const std::string& filename)
{
    Solution solution;
    std::ifstream solutionFile;
    solutionFile.open(filename.c_str(), std::ios::in);

    std::string line;
    getline(solutionFile, line);
    solutionFile.close();

    if(boost::contains(line, "-"))
    {
        return this->readBasicSolution_v2(filename);
    }
    return this->readBasicSolution_v1(filename);
}

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
